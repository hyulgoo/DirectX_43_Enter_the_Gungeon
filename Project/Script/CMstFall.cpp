#include "pch.h"
#include "CMstFall.h"
#include <Engine/CAnim2D.h>
#include "CWaveMgr.h"

CMstFall::CMstFall(UINT _MstType)
	: m_MstType(_MstType)
	, m_strAnimName{}
{
	SetAnimName();
}

CMstFall::~CMstFall()
{
}

void CMstFall::tick()
{
	if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish())
	{
		CWaveMgr::GetInst()->MinusMstCount();
		GetOwner()->SetLifeSpan(0.f);
	}
}

void CMstFall::Enter()
{
	PlayAnim();
}

void CMstFall::Exit()
{
}

void CMstFall::SetAnimName()
{
	switch ((MONSTER_TYPE)m_MstType)
	{
	case MONSTER_TYPE::REDBOOK:
	{
		m_strAnimName[(UINT)DIR::UP] = L"RedBook_Death";
		m_strAnimName[(UINT)DIR::DOWN] = L"RedBook_Death";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"RedBook_Death";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"RedBook_Death";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"RedBook_Death";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"RedBook_Death";
	}
	break;
	case MONSTER_TYPE::GREENBOOK:
	{
		m_strAnimName[(UINT)DIR::UP] = L"GreenBook_Death";
		m_strAnimName[(UINT)DIR::DOWN] = L"GreenBook_Death";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"GreenBook_Death";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"GreenBook_Death";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"GreenBook_Death";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"GreenBook_Death";
	}
	break;
	case MONSTER_TYPE::BULLETKIN:
	{
		m_strAnimName[(UINT)DIR::UP] = L"BulletKin_Fall_Left";
		m_strAnimName[(UINT)DIR::DOWN] = L"BulletKin_Fall_Left";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"BulletKin_Fall_Left";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"BulletKin_Fall_Left";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"BulletKin_Fall_Left";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"BulletKin_Fall_Left";
	}
	break;
	case MONSTER_TYPE::SHOTGUNKIN_BLUE:
	{
		m_strAnimName[(UINT)DIR::UP] = L"ShotgunKinBiue_Falling_Right";
		m_strAnimName[(UINT)DIR::DOWN] = L"ShotgunKinBiue_Falling_Right";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"ShotgunKinBiue_Falling_Right";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"ShotgunKinBiue_Falling_Right";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"ShotgunKinBiue_Falling_Right";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"ShotgunKinBiue_Falling_Right";
	}
	break;
	case MONSTER_TYPE::SHOTGUNKIN_RED:
	{
		m_strAnimName[(UINT)DIR::UP] = L"ShotgunKinRed_Falling_Left";
		m_strAnimName[(UINT)DIR::DOWN] = L"ShotgunKinRed_Falling_Left";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"ShotgunKinRed_Falling_Left";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"ShotgunKinRed_Falling_Left";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"ShotgunKinRed_Falling_Left";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"ShotgunKinRed_Falling_Left";
	}
	break;
	case MONSTER_TYPE::CUBULON:
	{
		m_strAnimName[(UINT)DIR::UP] = L"Cubulon_Death";
		m_strAnimName[(UINT)DIR::DOWN] = L"Cubulon_Death";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"Cubulon_Death";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"Cubulon_Death";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"Cubulon_Death";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"Cubulon_Death";
	}
	break;
	case MONSTER_TYPE::GIGI:
	{
		m_strAnimName[(UINT)DIR::UP] = L"Gigi_Death_Right";
		m_strAnimName[(UINT)DIR::DOWN] = L"Gigi_Death_Right";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"Gigi_Death_Right";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"Gigi_Death_Right";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"Gigi_Death_Right";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"Gigi_Death_Right";
	}
	break;
	case MONSTER_TYPE::SHOTBAT:
	{
		m_strAnimName[(UINT)DIR::UP] = L"ShotBat_Death";
		m_strAnimName[(UINT)DIR::DOWN] = L"ShotBat_Death";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"ShotBat_Death";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"ShotBat_Death";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"ShotBat_Death";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"ShotBat_Death";
	}
	break;
	case MONSTER_TYPE::BULLETBAT:
	{
		m_strAnimName[(UINT)DIR::UP] = L"BulletBat_Death";
		m_strAnimName[(UINT)DIR::DOWN] = L"BulletBat_Death";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"BulletBat_Death";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"BulletBat_Death";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"BulletBat_Death";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"BulletBat_Death";
	}
	break;
	case MONSTER_TYPE::MAGE:
	{
		m_strAnimName[(UINT)DIR::UP] = L"Mage_Death_Down_Left";
		m_strAnimName[(UINT)DIR::DOWN] = L"Mage_Death_Down_Left";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"Mage_Death_Down_Left";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"Mage_Death_Down_Left";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"Mage_Death_Down_Left";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"Mage_Death_Down_Left";
	}
	break;
	}
}

void CMstFall::PlayAnim()
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

