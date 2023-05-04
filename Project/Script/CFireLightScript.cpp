#include "pch.h"
#include "CFireLightScript.h"
#include <Engine/CRenderMgr.h>
#include <Engine/CLight2D.h>

CFireLightScript::CFireLightScript()
	: CScript((UINT)SCRIPT_TYPE::FIRELIGHTSCRIPT)
	, m_pParent(nullptr)
	, m_vParentPos{}
	, m_bSetLight(false)
	, m_fDelay(0.f)
{
}

CFireLightScript::~CFireLightScript()
{
}

void CFireLightScript::begin()
{
	if (nullptr == m_pParent)
		m_pParent = GetOwner()->GetParent();
	Light2D()->SetLightDiffuse(Vec3(0.f, 0.f, 0.f));
}

void CFireLightScript::tick()
{
	if (m_bSetLight)
	{
		m_fDelay -= DT;
		if (m_fDelay < 0.f)
		{
			Light2D()->SetLightDiffuse(Vec3(0.f, 0.f, 0.f));
			m_bSetLight = false;
		}
	}
	SetPos();
}

void CFireLightScript::SetPos()
{
	m_vParentPos = m_pParent->Transform()->GetWorldPos();
	Vec3 CurPos = Transform()->GetWorldPos();
	Vec3 TargetPos = m_vParentPos;
	Vec3 Cursor = CRenderMgr::GetInst()->GetRay();
	Vec2 vDir = Vec2(Cursor.x - TargetPos.x, Cursor.y - TargetPos.y);
	vDir.Normalize();
	float angle = (float)acos(vDir.Dot(Vec2(0.f, -1.f)));
	if (vDir.x < 0)
		angle = (360 / 180 * XM_PI) - angle;
	
	// 아래를 기준으로 한 각도가 180 이상이라면 (왼쪽을 보고 있다면)
	if (angle * (180.f / XM_PI) > 180.f)
		Transform()->SetRelativePos(Vec3(-vDir.x, vDir.y, 0.f));
	// 오른쪽을 보고있다면
	else
		Transform()->SetRelativePos(Vec3(vDir.x, vDir.y, 0.f));
}

void CFireLightScript::SetLightAngle(float _fAngle)
{
	Light2D()->SetAngle(_fAngle);
}

void CFireLightScript::SetLightDiffuse(float _fDiffuse)
{
	Light2D()->SetLightDiffuse(Vec3(_fDiffuse, _fDiffuse, _fDiffuse));
}

void CFireLightScript::SetLight(bool _bLight)
{
	m_bSetLight = _bLight;
	Light2D()->SetLightDiffuse(Vec3(1.2f, 1.2f, 1.2f));
	m_fDelay = 0.2f;
}
