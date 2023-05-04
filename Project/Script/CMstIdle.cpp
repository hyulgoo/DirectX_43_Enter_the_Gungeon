#include "pch.h"
#include "CMstIdle.h"
#include "CMonsterScript.h"
#include <Engine/CAnim2D.h>

CMstIdle::CMstIdle(UINT _MstType)
	: m_MstType(_MstType)
	, m_strAnimName{}
{
	SetAnimName();
}

CMstIdle::~CMstIdle()
{
}
void CMstIdle::tick()
{
	if (m_DetectRange > GetOwner()->GetScript<CMonsterScript>()->GetLengthtoPly())
		ChangeState(L"Trace");
}

void CMstIdle::Enter()
{
	tStatus Stat = GetOwnerScript()->GetStat();
	m_DetectRange = Stat.Detect_Range;
	PlayAnim();
}

void CMstIdle::Exit()
{
	GetOwner()->Animator2D()->GetCurAnim()->Reset();
}


void CMstIdle::SetAnimName()
{
	switch ((MONSTER_TYPE)m_MstType)
	{
	case MONSTER_TYPE::REDBOOK:
	{
		m_strAnimName[(UINT)DIR::UP] = L"RedBook_Idle";
		m_strAnimName[(UINT)DIR::DOWN] = L"RedBook_Idle";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"RedBook_Idle";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"RedBook_Idle";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"RedBook_Idle";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"RedBook_Idle";
	}
	break;
	case MONSTER_TYPE::GREENBOOK:
	{
		m_strAnimName[(UINT)DIR::UP] = L"GreenBook_Idle";
		m_strAnimName[(UINT)DIR::DOWN] = L"GreenBook_Idle";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"GreenBook_Idle";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"GreenBook_Idle";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"GreenBook_Idle";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"GreenBook_Idle";
	}
	break;
	case MONSTER_TYPE::BULLETKIN:
	{
		m_strAnimName[(UINT)DIR::UP] = L"BulletKin_Idle_Up";
		m_strAnimName[(UINT)DIR::DOWN] = L"BulletKin_Idle_Left";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"BulletKin_Idle_Left";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"BulletKin_Idle_Left";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"BulletKin_Idle_Left";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"BulletKin_Idle_Left";
	}
	break;
	case MONSTER_TYPE::SHOTGUNKIN_BLUE:
	{
		m_strAnimName[(UINT)DIR::UP] = L"ShotgunKinBiue_Idle_Up";
		m_strAnimName[(UINT)DIR::DOWN] = L"ShotgunKinBiue_Idle_Down";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"ShotgunKinBiue_Idle_Down";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"ShotgunKinBiue_Idle_Down";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"ShotgunKinBiue_Idle_Down";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"ShotgunKinBiue_Idle_Down";
	}
	break;
	case MONSTER_TYPE::SHOTGUNKIN_RED:
	{
		m_strAnimName[(UINT)DIR::UP] = L"ShotgunKinRed_Idle_Up";
		m_strAnimName[(UINT)DIR::DOWN] = L"ShotgunKinRed_Idle_Down";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"ShotgunKinRed_Idle_Down";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"ShotgunKinRed_Idle_Down";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"ShotgunKinRed_Idle_Down";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"ShotgunKinRed_Idle_Down";
	}
	break;
	case MONSTER_TYPE::CUBULON:
	{
		m_strAnimName[(UINT)DIR::UP] = L"Cubulon_Idle_Right";
		m_strAnimName[(UINT)DIR::DOWN] = L"Cubulon_Idle_Right";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"Cubulon_Idle_Right";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"Cubulon_Idle_Right";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"Cubulon_Idle_Right";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"Cubulon_Idle_Right";
	}
	break;
	case MONSTER_TYPE::GIGI:
	{
		m_strAnimName[(UINT)DIR::UP] = L"Gigi_Idle_Right";
		m_strAnimName[(UINT)DIR::DOWN] = L"Gigi_Idle_Right";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"Gigi_Idle_Right";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"Gigi_Idle_Right";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"Gigi_Idle_Right";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"Gigi_Idle_Right";
	}
	break;
	case MONSTER_TYPE::SHOTBAT:
	{
		m_strAnimName[(UINT)DIR::UP] = L"ShotBat_Idle";
		m_strAnimName[(UINT)DIR::DOWN] = L"ShotBat_Idle";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"ShotBat_Idle";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"ShotBat_Idle";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"ShotBat_Idle";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"ShotBat_Idle";
	}
	break;
	case MONSTER_TYPE::BULLETBAT:
	{
		m_strAnimName[(UINT)DIR::UP] = L"BulletBat_Idle";
		m_strAnimName[(UINT)DIR::DOWN] = L"BulletBat_Idle";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"BulletBat_Idle";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"BulletBat_Idle";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"BulletBat_Idle";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"BulletBat_Idle";
	}
	break;
	case MONSTER_TYPE::MAGE:
	{
		m_strAnimName[(UINT)DIR::UP] = L"Mage_Idle_Down_Left";
		m_strAnimName[(UINT)DIR::DOWN] = L"Mage_Idle_Down_Left";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"Mage_Idle_Down_Left";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"Mage_Idle_Down_Left";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"Mage_Idle_Down_Left";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"Mage_Idle_Down_Left";
	}
	break;
	}
}

void CMstIdle::PlayAnim()
{
	CAnimator2D* Anim = GetOwner()->Animator2D();
	switch (GetDir())
	{
	case DIR::UP:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::UP], true);
		break;
	case DIR::DOWN:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::DOWN], true);
		break;
	case DIR::LEFT_UP:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::LEFT_UP], true);
		break;
	case DIR::LEFT_DOWN:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::LEFT_DOWN], true);
		break;
	case DIR::RIGHT_UP:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::RIGHT_UP], true);
		break;
	case DIR::RIGHT_DOWN:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::RIGHT_DOWN], true);
		break;
	}
}
