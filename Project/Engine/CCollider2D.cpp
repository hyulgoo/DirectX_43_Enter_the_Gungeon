#include "pch.h"
#include "CCollider2D.h"

#include "CScript.h"
#include "components.h"

CCollider2D::CCollider2D()
	:CComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_Shape(COLLIDER2D_TYPE::RECT)
	, m_bAbsolute(false)
	, m_iCollisionCount(0)
	, m_bPush(false)
{
	SetName(L"Collider2D");
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::finaltick()
{
	assert(0 <= m_iCollisionCount);

	// 상대크기를 크기행렬로 세팅.
	m_matCollider2D = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);
	// 크기행렬에 이동좌표를 곱함.
	m_matCollider2D *= XMMatrixTranslation(m_vOffsetPos.x,m_vOffsetPos.y, m_vOffsetPos.z);

	// 소유 오브젝트의 월드행렬을 가져옴
	const Matrix& matWorld = Transform()->GetWorldMat();

	// 절대값 적용 유무에 따른 분기처리
	if (m_bAbsolute)
	{
		// 누적 크기값의 역행렬을 곱해 누적 크기값을 제외시킴
		Matrix matParentScaleInv = XMMatrixInverse(nullptr, Transform()->GetWorldScaleMat());
		m_matCollider2D = m_matCollider2D * matParentScaleInv * matWorld;
	}
	else
	{
		// 충돌체 월드 * 오브젝트 월드
		m_matCollider2D *= matWorld;
	}
	
	// DebugShape 요청
	Vec4 vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	if (0 < m_iCollisionCount)
		vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	if (COLLIDER2D_TYPE::CIRCLE == m_Shape)
		DrawDebugCircle(m_matCollider2D, vColor, 0.f);
	else
		DrawDebugRect(m_matCollider2D, vColor, 0.f);
}

void CCollider2D::BeginOverlap(CCollider2D* _Other)
{
	++m_iCollisionCount;
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->BeginOverlap(_Other);
	}
	if (m_bPush)
	{
		if (0 != m_CollisionArea.x || 0 != m_CollisionArea.y)
		{
			Transform()->SetRelativePos(Transform()->GetRelativePos() + m_CollisionArea);
			m_CollisionArea.x = 0.f;
			m_CollisionArea.y = 0.f;
		}
	}
}

void CCollider2D::OnOverlap(CCollider2D* _Other)
{
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnOverlap(_Other);
	}
	if(m_bPush)
	{
		if (0 != m_CollisionArea.x || 0 != m_CollisionArea.y)
		{
			Transform()->SetRelativePos(Transform()->GetRelativePos() + m_CollisionArea);
			m_CollisionArea.x = 0.f;
			m_CollisionArea.y = 0.f;
		}
	}
}

void CCollider2D::EndOverlap(CCollider2D* _Other)
{
	--m_iCollisionCount; 
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->EndOverlap(_Other);
	}
}

void CCollider2D::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_vOffsetPos,   sizeof(Vec3), 1, _File);
	fwrite(&m_vOffsetScale, sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute,    sizeof(bool), 1, _File);
	fwrite(&m_Shape, sizeof(UINT), 1, _File);
}

void CCollider2D::LoadFromLevelFile(FILE* _File)
{
	fread(&m_vOffsetPos,   sizeof(Vec3), 1, _File);
	fread(&m_vOffsetScale, sizeof(Vec3), 1, _File);
	fread(&m_bAbsolute,    sizeof(bool), 1, _File);
	fread(&m_Shape, sizeof(UINT), 1, _File);
}
