#include "pch.h"
#include "CLight2DScript.h"

#include <Engine/CLight2D.h>

CLight2DScript::CLight2DScript()
	: CScript((UINT)SCRIPT_TYPE::LIGHT2DSCRIPT)
{
	SetName(L"LightEffect");
}

CLight2DScript::~CLight2DScript()
{
}

void CLight2DScript::tick()
{
	tLightInfo tLightInfo = m_pOwner->Light2D()->GetLightInfo();
	if (KEY_PRESSED(KEY::C))
	{
		Vec3 vRot = Transform()->GetRelativeRot();
		vRot.z += DT * (XM_PI);
		Transform()->SetRelativeRot(vRot);
	}
	if (KEY_TAP(KEY::V))
	{
		if (tLightInfo.LightType == (UINT)LIGHT_TYPE::SPOT)
		{
			m_pOwner->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
		}
		else
			m_pOwner->Light2D()->SetLightType((LIGHT_TYPE)((UINT)tLightInfo.LightType + 1));
	}
	if (KEY_TAP(KEY::X))
	{
		Vec4 vDiffuse = m_pOwner->Light2D()->GetLightInfo().Color.vDiffuse;
		Vec4 Diff = Vec4(0.f + 100 * DT, 0.f + 100 * DT, 0.f + 100 * DT, 0.f + 100 * DT);
		m_pOwner->Light2D()->SetLightDiffuse(vDiffuse + Diff);
	}

}
