#include "pch.h"
#include "CWpIdle.h"
#include "CUIMgr.h"

CWpIdle::CWpIdle()
{
}

CWpIdle::~CWpIdle()
{
}

void CWpIdle::tick()
{
	if (KEY_TAP(KEY::LBTN))
		ChangeState(L"Attack");
}

void CWpIdle::Enter()
{
	PlayAnim();
	CUIMgr::GetInst()->PlayWeaponAnim(WEAPON_STATES::IDLE);
}

void CWpIdle::Exit()
{
}

void CWpIdle::PlayAnim()
{
	switch (GetType())
	{
	case WEAPON_TYPE::OLDGUN:
		GetOwner()->Animator2D()->Play(L"Oldgun_idle", false);
		break;
	case WEAPON_TYPE::CROSSBOW:
		GetOwner()->Animator2D()->Play(L"Crossbow_idle", false);
		break;
	case WEAPON_TYPE::MAGNUM:
		GetOwner()->Animator2D()->Play(L"Magnum_idle", false);
		break;
	}
}
