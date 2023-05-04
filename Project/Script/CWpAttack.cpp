#include "pch.h"
#include "CWpAttack.h"
#include "CUIMgr.h"

CWpAttack::CWpAttack()
	: m_fDelay(0.f)
{
}

CWpAttack::~CWpAttack()
{
}

void CWpAttack::tick()
{
	m_fDelay -= DT;
	if (m_fDelay <= 0.f)
		GetOwnerScript()->ChangeState(L"Idle");
}

void CWpAttack::Enter()
{
	PlayAnim();
	CUIMgr::GetInst()->PlayWeaponAnim(WEAPON_STATES::SHOT);
}

void CWpAttack::Exit()
{
}

void CWpAttack::PlayAnim()
{
	switch (GetType())
	{
	case WEAPON_TYPE::OLDGUN:
	{
		GetOwner()->Animator2D()->Play(L"Oldgun_fire", true);
		CResMgr::GetInst()->FindRes<CSound>(L"OldGun_Shot")->Play(1, 1, true);
		m_fDelay = 0.2f;
	}
	break;
	case WEAPON_TYPE::CROSSBOW:
	{
		GetOwner()->Animator2D()->Play(L"Crossbow_fire", true);
		CResMgr::GetInst()->FindRes<CSound>(L"CrossBow_Shot")->Play(1, 1, true);
		m_fDelay = 0.01f;
	}
	break;
	case WEAPON_TYPE::MAGNUM:
	{
		GetOwner()->Animator2D()->Play(L"Magnum_fire", true);
		CResMgr::GetInst()->FindRes<CSound>(L"OldGun_Shot")->Play(1, 1, true);
		m_fDelay = 0.2f;
	}
	break;
	}
}
