#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f,1.f,1.f))
	, m_bAbsolute(false)
	, m_vRelativeDir{
		 Vec3(1.f,0.f,0.f)
		,Vec3(0.f,1.f,0.f)
		,Vec3(0.f,0.f,1.f)}
{
	SetName(L"Transform");
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{
	m_vPrevPos = m_matWorld.Translation();

	// 크기행렬
	m_matWorldScale = XMMatrixIdentity();
	m_matWorldScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

	// 회전 행렬
	Matrix matRot = XMMatrixIdentity();
	matRot = XMMatrixRotationX(m_vRelativeRot.x);
	matRot *= XMMatrixRotationY(m_vRelativeRot.y);
	matRot *= XMMatrixRotationZ(m_vRelativeRot.z);

	// 위치행렬
	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	// World View에 각 행렬을 곱함. 반드시 크기, 회전, 이동 순으로 할 것
	m_matWorld = m_matWorldScale * matRot * matTranslation;

	Vec3 vDefaultDir[3] = {
		  Vec3(1.f, 0.f, 0.f)
		, Vec3(0.f, 1.f, 0.f)
		, Vec3(0.f, 0.f, 1.f)
	};

	for (int i = 0; i < 3; ++i)
	{
		m_vWorldDir[i] = m_vRelativeDir[i] = XMVector3TransformNormal(vDefaultDir[i], matRot);
	}

	// 부모 오브젝트를 확인하고 있다면 자식의 월드행렬에 부모의 월드행렬을 곱함.
	CGameObject* pParent = GetOwner()->GetParent();
	if (pParent)
	{
		if (m_bAbsolute)
		{
			Matrix matParentWorld = pParent->Transform()->m_matWorld;
			Matrix matParentScale = pParent->Transform()->m_matWorldScale;
			// 크기행렬의 역행렬
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale);

			// 크기를 적용하지 않아도 누적된 크기값은 받아둠(자식은 누적값을 적용받게 할 수도 있어야하므로)
			//m_matWorldScale = pParent->Transform()->m_matWorldScale;
			// 월드행렬 = 로컬월드 * 부모 크기행렬의 역행렬 * 부모 월드(크기/회전/이동)
			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}
		else
		{
			// 크기를 적용하지 않아도 누적된 크기값은 받아둠
			m_matWorldScale *= pParent->Transform()->m_matWorldScale;
			m_matWorld *= pParent->Transform()->m_matWorld;
		}

		for (int i = 0; i < 3; ++i)
		{
			// TransformNomal로 이동값 제외
			m_vWorldDir[i] = XMVector3TransformNormal(vDefaultDir[i], m_matWorld);
			// Nomalize로 크기값 제외
			m_vWorldDir[i].Normalize();
		}
	}

}

void CTransform::UpdateData()
{
	CConstBuffer* pTransformBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);

	g_transform.matWorld = m_matWorld;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;

	pTransformBuffer->SetData(&g_transform);
	pTransformBuffer->UpdateData();
}

void CTransform::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_vRelativePos,   sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeRot,   sizeof(Vec3), 1, _File);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _File);
}

void CTransform::LoadFromLevelFile(FILE* _File)
{
	fread(&m_vRelativePos,   sizeof(Vec3), 1, _File);
	fread(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fread(&m_vRelativeRot,   sizeof(Vec3), 1, _File);
	fread(&m_bAbsolute, sizeof(bool), 1, _File);
}
