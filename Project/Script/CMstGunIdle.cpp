#include "pch.h"
#include "CMstGunIdle.h"
#include <Engine/CAnim2D.h>
#include "CMonsterScript.h"

CMstGunIdle::CMstGunIdle(UINT _MstType)
	: m_Type(_MstType)
{
}

CMstGunIdle::~CMstGunIdle()
{
}

void CMstGunIdle::tick()
{
}

void CMstGunIdle::Enter()
{
	PlayAnim();
}

void CMstGunIdle::Exit()
{
	GetOwner()->Animator2D()->GetCurAnim()->Reset();
}

void CMstGunIdle::PlayAnim()
{
	
	switch ((MONSTER_TYPE)GetOwner()->GetParent()->GetScript<CMonsterScript>()->GetType())
	{
	case MONSTER_TYPE::BULLETKIN:
		GetOwner()->Animator2D()->Play(L"Magnum_idle", false);
		break;
	case MONSTER_TYPE::SHOTGUNKIN_BLUE:
		GetOwner()->Animator2D()->Play(L"Shotgun_idle", false);
		break;
	case MONSTER_TYPE::SHOTGUNKIN_RED:
		GetOwner()->Animator2D()->Play(L"Shotgun_idle", false);
		break;
	}
}
