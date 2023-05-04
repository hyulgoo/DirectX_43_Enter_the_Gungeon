#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CCollider2D.h"

CCollisionMgr::CCollisionMgr()
	: m_matrix{}
{

}

CCollisionMgr::~CCollisionMgr()
{

}

void CCollisionMgr::tick()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT iRow = 0; iRow < MAX_LAYER; ++iRow)
	{
		for (UINT iCol = iRow; iCol < MAX_LAYER; ++iCol)
		{
			if (!(m_matrix[iRow] & (1 << iCol)))
				continue;

			CollisionBtwLayer(pLevel->GetLayer(iRow), pLevel->GetLayer(iCol));
		}
	}
}

void CCollisionMgr::CollisionBtwLayer(CLayer* _LeftLayer, CLayer* _RightLayer)
{
	const vector<CGameObject*>& vecLeft = _LeftLayer->GetObjects();
	const vector<CGameObject*>& vecRight = _RightLayer->GetObjects();
	
	// 같은 레이어라면 중복검사를 생략
	if (_LeftLayer == _RightLayer)
	{
		for (size_t i = 0; i < vecLeft.size(); ++i)
		{
			for (size_t j = i + 1; j < vecRight.size(); ++j)
			{
				CollisionBtwObject
				(vecLeft[i], vecRight[j]);
			}
		}
	}
	else
	{
		for (size_t i = 0; i < vecLeft.size(); ++i)
		{
			for (size_t j = 0; j < vecRight.size(); ++j)
			{
				CollisionBtwObject(vecLeft[i], vecRight[j]);
			}
		}
	}
}

#include "CTransform.h"

void CCollisionMgr::CollisionBtwObject(CGameObject* _LeftObject, CGameObject* _RightObject)
{
	if (!(_LeftObject->Collider2D() && _RightObject->Collider2D()))
		return;

	CollisionID id = {};
	id.LeftID = _LeftObject->Collider2D()->GetID();
	id.RightID = _RightObject->Collider2D()->GetID();

	map<UINT_PTR, bool>::iterator iter = m_mapColID.find(id.id);
	// 충돌 아이디가 없다는 것은 이전에 충돌한 적이 없다는 것
	// 아이디를 만들어주고 이전 충돌 정보를 false로 해줌
	if (iter == m_mapColID.end())
	{
		m_mapColID.insert(make_pair(id.id, false));
		// 찾아서 없다면 만들고 다시 찾기 때문에 반드시 있음.
		iter = m_mapColID.find(id.id);
	}
	
	// 둘 중 하나라도 삭제 예정이라면 (Dead 상태라면)
	bool bDead = false;
	if (_LeftObject->IsDead() || _RightObject->IsDead())
	{
		bDead = true;
	}

	// 두 물체가 충돌 중이라면
	if (CollisionBtwCollider(_LeftObject->Collider2D(), _RightObject->Collider2D()))
	{
		// 둘 중 하나가 Dead 상태라면 충돌을 끝낸다.
		if (bDead && iter->second)
		{
			_LeftObject->Collider2D()->EndOverlap(_RightObject->Collider2D());
			_RightObject->Collider2D()->EndOverlap(_LeftObject->Collider2D());
		}
		// 계속 충돌 중이라면
		else if (iter->second)
		{
			_LeftObject->Collider2D()->OnOverlap(_RightObject->Collider2D());
			_RightObject->Collider2D()->OnOverlap(_LeftObject->Collider2D());
		}
		// 이번에 충돌하는 거라면
		else
		{
			// 둘 중 하나가 Dead 상태라면 충돌을 없었던 것으로 한다.
			if (!bDead)
			{
				_LeftObject->Collider2D()->BeginOverlap(_RightObject->Collider2D());
				_RightObject->Collider2D()->BeginOverlap(_LeftObject->Collider2D());
				iter->second = true;
			}
		}
	}
	else
	{	
		// 충돌 해제
		// 이전에 충돌한 적이 없다면 아무 상태도 아니므로 제외한다.
		if (iter->second)
		{
			_LeftObject->Collider2D()->EndOverlap(_RightObject->Collider2D());
			_RightObject->Collider2D()->EndOverlap(_LeftObject->Collider2D());
			iter->second = false;
		}
	}
}

bool CCollisionMgr::CollisionBtwCollider(CCollider2D* _pLeft, CCollider2D* _pRight)
{
	// 0 -- 1
	// |    |
	// 3 -- 2
	Vec3 arrLocal[4] =
	{
		Vec3(-0.5f, 0.5f, 0.f),
		Vec3(0.5f, 0.5f, 0.f),
		Vec3(0.5f, -0.5f, 0.f),
		Vec3(-0.5f, -0.5f, 0.f),
	};
	
	Vec3 arrProj[4] = {};

	arrProj[0] = XMVector3TransformCoord(arrLocal[1], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat());
	arrProj[1] = XMVector3TransformCoord(arrLocal[3], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat());

	arrProj[2] = XMVector3TransformCoord(arrLocal[1], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat());
	arrProj[3] = XMVector3TransformCoord(arrLocal[3], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat());

	Vec3 vCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pLeft->GetColliderWorldMat());

	float CollisionArea[4] = {};
	// 분리축 테스트
	for (int i = 0; i < 4; ++i)
	{
		Vec3 vProj = arrProj[i];
		vProj.Normalize();

		// 4개의 표면벡터를 지정된 투영축으로 이동시킨 거리의 총합 / 2 (합친 길이의 절반이 중심점으로부터의 거리이므로)
		float fProjDist = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			fProjDist += fabsf(arrProj[j].Dot(vProj));
		}
		fProjDist /= 2.f;

		float fCenter = fabsf(vCenter.Dot(vProj));

		if (fProjDist < fCenter)
			return false;
		CollisionArea[i] = fProjDist - fCenter;
	}
	Vec3 vLeftMove = _pLeft->Transform()->GetWorldPos() - _pLeft->Transform()->GetPrevPos();
	Vec3 vRightMove = _pRight->Transform()->GetWorldPos() - _pRight->Transform()->GetPrevPos();
	float LeftMoveScale = fabs(vLeftMove.x) + fabs(vLeftMove.y);
	float RightMoveScale = fabs(vRightMove.x) + fabs(vRightMove.y);
	Vec2 LeftRealMove = Vec2(vLeftMove.x - CollisionArea[0], vLeftMove.y - CollisionArea[1]);
	Vec2 RightRealMove = Vec2(vRightMove.x - CollisionArea[2], vRightMove.y - CollisionArea[3]);
	if(LeftMoveScale >= RightMoveScale && LeftRealMove.x != 0.f || LeftRealMove.y != 0.f)
		CheckCollisionDir(_pLeft, _pRight, CollisionArea);
	else if (LeftMoveScale < RightMoveScale && RightRealMove.x != 0.f || RightRealMove.y != 0.f)
		CheckCollisionDir(_pRight, _pLeft, CollisionArea);

	return true;
}

void CCollisionMgr::CheckCollisionDir(CCollider2D* _pLeft, CCollider2D* _pRight, float _vColiisionArea[4])
{
	Vec3 vLeftMove = _pLeft->Transform()->GetWorldPos() - _pLeft->Transform()->GetPrevPos();

	if (vLeftMove.x < 0.f && _vColiisionArea[2] < _vColiisionArea[3])
	{
		_pRight->SetCollisionArea(Vec2(-(_vColiisionArea[0]), 0.f));
		_pRight->SetDir(COLLISION_DIR::RIGHT);
		_pLeft->SetCollisionArea(Vec2( (_vColiisionArea[2]), 0.f));
		_pLeft->SetDir(COLLISION_DIR::LEFT);
	}
	else if (vLeftMove.x > 0.f && _vColiisionArea[2] < _vColiisionArea[3])
	{
		_pRight->SetCollisionArea(Vec2((_vColiisionArea[0]), 0.f));
		_pRight->SetDir(COLLISION_DIR::LEFT);
		_pLeft->SetCollisionArea(Vec2(-(_vColiisionArea[2]), 0.f));
		_pLeft->SetDir(COLLISION_DIR::RIGHT);
	}

	else if (vLeftMove.y < 0.f && _vColiisionArea[2] > _vColiisionArea[3])
	{
		_pRight->SetCollisionArea(Vec2(0.f, -(_vColiisionArea[1])));
		_pRight->SetDir(COLLISION_DIR::UP);
		_pLeft->SetCollisionArea(Vec2(0.f, (_vColiisionArea[3])));
		_pLeft->SetDir(COLLISION_DIR::DOWN);
	}
	else if (vLeftMove.y > 0.f && _vColiisionArea[2] > _vColiisionArea[3])
	{
		_pRight->SetCollisionArea(Vec2(0.f, (_vColiisionArea[1])));
		_pRight->SetDir(COLLISION_DIR::DOWN);
		_pLeft->SetCollisionArea(Vec2(0.f, -(_vColiisionArea[3])));
		_pLeft->SetDir(COLLISION_DIR::UP);
	}
}

void CCollisionMgr::LayerCheck(UINT _left, UINT _right)
{
	// 중복검사를 막기 위해서 횡이 열보다 작으면 하지않음.
	UINT iRow = _left;
	UINT iCol = _right;

	if (iRow > iCol)
	{
		UINT iTemp = iCol;
		iCol = iRow;
		iRow = iTemp;
	}
	m_matrix[iRow] |= (1 << iCol);
}

void CCollisionMgr::LayerCheck(const wstring& _strLeftLayer, const wstring& _strRightLayer)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	CLayer* pLeft = pCurLevel->FindLayerByName(_strLeftLayer);
	CLayer* pRight = pCurLevel->FindLayerByName(_strRightLayer);

	LayerCheck(pLeft->GetLayerIdx(), pRight->GetLayerIdx());

}
