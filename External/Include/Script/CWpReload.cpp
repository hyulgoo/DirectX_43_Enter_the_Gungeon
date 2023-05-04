#include "pch.h"
#include "CWpReload.h"
#include <Engine/CAnim2D.h>
#include "CUIMgr.h"

CWpReload::CWpReload()
	: m_fDelay(0.f)
{
}

CWpReload::~CWpReload()
{
}

void CWpReload::tick()
{
	m_fDelay -= DT;
	if (m_fDelay <= 0.f)
		GetOwnerScript()->ChangeState(L"Idle");
}

void CWpReload::Enter()
{
	PlayAnim();
	CUIMgr::GetInst()->PlayWeaponAnim(WEAPON_STATES::RELOAD);
}

void CWpReload::Exit()
{
}

void CWpReload::PlayAnim()
{
	switch (GetType())
	{
	case WEAPON_TYPE::OLDGUN:
		GetOwner()->Animator2D()->Play(L"Oldgun_reload_start", true);
		CResMgr::GetInst()->FindRes<CSound>(L"OldGun_Reload")->Play(1, 1, false);
		m_fDelay = 1.5f;
		break;
	case WEAPON_TYPE::CROSSBOW:
		GetOwner()->Animator2D()->Play(L"Crossbow_reload", false);
		CResMgr::GetInst()->FindRes<CSound>(L"CrossBow_Reload")->Play(1, 1, false);
		m_fDelay = 0.75f;
		break;
	case WEAPON_TYPE::MAGNUM:
		GetOwner()->Animator2D()->Play(L"Oldgun_reload_start", false);
		CResMgr::GetInst()->FindRes<CSound>(L"OldGun_Reload")->Play(1, 1, false);
		m_fDelay = 0.2f;
		break;
	}
}
