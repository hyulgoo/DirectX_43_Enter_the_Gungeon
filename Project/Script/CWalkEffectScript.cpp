#include "pch.h"
#include "CWalkEffectScript.h"

CWalkEffectScript::CWalkEffectScript()
	: CScript((UINT)SCRIPT_TYPE::WALKEFFECTSCRIPT)
	, m_pPlayer(nullptr)
{
}

CWalkEffectScript::CWalkEffectScript(const CWalkEffectScript& _Other)
	: CScript((UINT)SCRIPT_TYPE::WALKEFFECTSCRIPT)
	, m_pPlayer(nullptr)
{
}

CWalkEffectScript::~CWalkEffectScript()
{
}
void CWalkEffectScript::begin()
{
	if (nullptr == m_pPlayer)
		m_pPlayer = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player");
	ParticleSystem()->OnOff(false);
}

void CWalkEffectScript::tick()
{
	if (nullptr == m_pPlayer)
		return;
	Vec3 vPlayerPos = m_pPlayer->Transform()->GetWorldPos();
	Transform()->SetRelativePos(Vec3(vPlayerPos.x, vPlayerPos.y - 30.f, vPlayerPos.z));
}

