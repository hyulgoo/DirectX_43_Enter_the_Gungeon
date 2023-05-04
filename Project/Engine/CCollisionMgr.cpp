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
	
	// ���� ���̾��� �ߺ��˻縦 ����
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
	// �浹 ���̵� ���ٴ� ���� ������ �浹�� ���� ���ٴ� ��
	// ���̵� ������ְ� ���� �浹 ������ false�� ����
	if (iter == m_mapColID.end())
	{
		m_mapColID.insert(make_pair(id.id, false));
		// ã�Ƽ� ���ٸ� ����� �ٽ� ã�� ������ �ݵ�� ����.
		iter = m_mapColID.find(id.id);
	}
	
	// �� �� �ϳ��� ���� �����̶�� (Dead ���¶��)
	bool bDead = false;
	if (_LeftObject->IsDead() || _RightObject->IsDead())
	{
		bDead = true;
	}

	// �� ��ü�� �浹 ���̶��
	if (CollisionBtwCollider(_LeftObject->Collider2D(), _RightObject->Collider2D()))
	{
		// �� �� �ϳ��� Dead ���¶�� �浹�� ������.
		if (bDead && iter->second)
		{
			_LeftObject->Collider2D()->EndOverlap(_RightObject->Collider2D());
			_RightObject->Collider2D()->EndOverlap(_LeftObject->Collider2D());
		}
		// ��� �浹 ���̶��
		else if (iter->second)
		{
			_LeftObject->Collider2D()->OnOverlap(_RightObject->Collider2D());
			_RightObject->Collider2D()->OnOverlap(_LeftObject->Collider2D());
		}
		// �̹��� �浹�ϴ� �Ŷ��
		else
		{
			// �� �� �ϳ��� Dead ���¶�� �浹�� ������ ������ �Ѵ�.
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
		// �浹 ����
		// ������ �浹�� ���� ���ٸ� �ƹ� ���µ� �ƴϹǷ� �����Ѵ�.
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
	// �и��� �׽�Ʈ
	for (int i = 0; i < 4; ++i)
	{
		Vec3 vProj = arrProj[i];
		vProj.Normalize();

		// 4���� ǥ�麤�͸� ������ ���������� �̵���Ų �Ÿ��� ���� / 2 (��ģ ������ ������ �߽������κ����� �Ÿ��̹Ƿ�)
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
	// �ߺ��˻縦 ���� ���ؼ� Ⱦ�� ������ ������ ��������.
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
