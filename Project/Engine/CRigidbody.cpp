#include "pch.h"
#include "CRigidbody.h"
#include "CTimeMgr.h"
#include "CTransform.h"


CRigidbody::CRigidbody()
	: CComponent(COMPONENT_TYPE::RIGIDBODY)
	, m_fMass(1.f)
	, m_fFriction(400.f)
	, m_fFrictionScale(3.f)
	, m_fVelocityLimit(1000.f)
	, m_fGravityVLimit(1000.f)
	, m_bGravityUse(false)
	, m_fGravityAccel(500.f)
	, m_bGround(false)

{
}

CRigidbody::~CRigidbody()
{
}

void CRigidbody::finaltick()
{
	// ���ӵ� = �� x ����
	Vec2 vAccel = m_vForce * m_fMass;

	// ���ӵ��� ���߽��� �ӵ��� ������Ŵ
	m_vVelocity += vAccel * DT;

	if (m_bGravityUse && m_bGround || !m_bGravityUse)
	{ //���� ���ӵ�
		Vec2 vFriction = -m_vVelocity;

		// ���� ������ 0�� �ƴ϶��
		if (!(0 == vFriction.x && 0 == vFriction.y))
		{
			// ������ = ������ * ������� * ���� * �ð�
			vFriction.Normalize();
			vFriction *= (m_fFriction * m_fFrictionScale * m_fMass * DT);
		}

		// ���� ũ��(����)�� �� �� �������� ũ�Ⱑ �� ��ٸ� ���� 0����
		if (m_vVelocity.Length() < vFriction.Length())
		{
			m_vVelocity = Vec2(0.f, 0.f);
		}

		// ���ӵ��� �������� ����
		m_vVelocity += vFriction;
	}
	// ���߻����̸� �߷� ������ true�� �� �߷� ����
	if (m_bGravityUse && !m_bGround)
	{
		Vec2 vGravityAccel = Vec2(0.f, m_fGravityAccel);
		m_vVelocity += vGravityAccel * DT;
	}

	// ���Ѽӵ�

	 // �߷��� �ۿ� ���� �� x���� �Ϲ����� ���Ѽӵ� y���� �߷����Ѽӵ�
	if (m_bGravityUse)
	{
		if (m_fVelocityLimit < fabsf(m_vVelocity.x))
		{
			m_vVelocity.x = m_vVelocity.x / fabsf(m_vVelocity.x) * m_fVelocityLimit;
		}

		else if (m_fGravityVLimit < fabsf(m_vVelocity.y))
		{
			m_vVelocity.y = m_vVelocity.y / fabsf(m_vVelocity.y) * m_fGravityVLimit;
		}
	}
	// �߷��� �ۿ������� ������ �Ϲ����� ���Ѽӵ� ����
	else
	{
		if (m_fVelocityLimit < m_vVelocity.Length())
		{
			m_vVelocity.Normalize();
			m_vVelocity *= m_fVelocityLimit;
		}
	}

	// ���������� ���� �ӵ��� DT���� �����Ŵ
	Vec3 vPos = Transform()->GetRelativePos();

	vPos.x += m_vVelocity.x * DT;
	vPos.y += m_vVelocity.y * DT;

	Transform()->SetRelativePos(vPos);

	m_vForce = Vec2(0.f, 0.f);
}

void CRigidbody::SetGround(bool _bGround)
{
	m_bGround = _bGround;

	if (m_bGround)
	{
		if (m_vVelocity.y > 0.f)
			m_vVelocity.y = 0;

	}
}

void CRigidbody::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fMass, sizeof(float), 1, _File);;
	fwrite(&m_fFriction, sizeof(float), 1, _File);;
	fwrite(&m_fFrictionScale, sizeof(float), 1, _File);;
	fwrite(&m_fVelocityLimit, sizeof(float), 1, _File);;
	fwrite(&m_fGravityAccel, sizeof(float), 1, _File);;
	fwrite(&m_fGravityVLimit, sizeof(float), 1, _File);;
	fwrite(&m_bGravityUse, sizeof(bool), 1, _File);;
	fwrite(&m_bGround, sizeof(bool), 1, _File);;
}

void CRigidbody::LoadFromLevelFile(FILE* _File)
{
	fread(&m_fMass, sizeof(float), 1, _File);;
	fread(&m_fFriction, sizeof(float), 1, _File);;
	fread(&m_fFrictionScale, sizeof(float), 1, _File);;
	fread(&m_fVelocityLimit, sizeof(float), 1, _File);;
	fread(&m_fGravityAccel, sizeof(float), 1, _File);;
	fread(&m_fGravityVLimit, sizeof(float), 1, _File);;
	fread(&m_bGravityUse, sizeof(bool), 1, _File);;
	fread(&m_bGround, sizeof(bool), 1, _File);;
}
