#include "pch.h"
#include "CMstChase.h"
#include "CMonsterScript.h"
#include <Engine/CAstarMgr.h>
#include <Engine/CAnim2D.h>

CMstChase::CMstChase(UINT _MstType)
	: m_MstType(_MstType)
	, m_strAnimName{}
{
	SetAnimName();
}

CMstChase::~CMstChase()
{
}

void CMstChase::tick()
{
	CMonsterScript* OwnerMstScript = GetOwner()->GetScript<CMonsterScript>();

	if (m_AttackRange > OwnerMstScript->GetLengthtoPly()
		&& OwnerMstScript->PossibleATK())
		ChangeState(L"Attack");
	else if(m_DetectRange < OwnerMstScript->GetLengthtoPly()
		|| !OwnerMstScript->PossibleATK())
	{
		float s = OwnerMstScript->GetLengthtoPly();
		ChangeState(L"Idle");
		int a = 0;
	}

	if (OwnerMstScript->GetLengthtoPly() < 150.f)
		return;
	Vec3 Pos = GetOwner()->Transform()->GetWorldPos();
	Vec3 PlayerPos = OwnerMstScript->GetPlayerPos();

	// A*로 Player에게 가는 다음 타일을 찾고 목적지로 삼음 
	Vec2 DestPos = CAstarMgr::GetInst()->SetFind(Pos, PlayerPos);
	if (0.f == DestPos.x && 0.f == DestPos.y)
		return;

	// 목적지 까지의 방향 * 스피드 * DT로 틱 당 이동시킴
	Vec2 DestDir = Vec2(DestPos.x - Pos.x, DestPos.y - Pos.y);
	DestDir.Normalize();
	Vec2 vDir = DestDir;
	Vec3 FinalPos = Vec3(Pos.x + vDir.x * GetOwnerScript()->GetStat().Speed * DT
		, Pos.y + vDir.y * GetOwnerScript()->GetStat().Speed * DT, Pos.z);
	GetOwner()->Transform()->SetRelativePos(FinalPos);

}

void CMstChase::Enter()
{
	tStatus Stat = GetOwnerScript()->GetStat();
	m_AttackRange = Stat.Attack_Range;
	m_DetectRange = Stat.Detect_Range;
	PlayAnim();
}

void CMstChase::Exit()
{
	GetOwner()->Animator2D()->GetCurAnim()->Reset();
}

void CMstChase::SetAnimName()
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
		m_strAnimName[(UINT)DIR::UP] = L"BulletKin_Walk_Up";
		m_strAnimName[(UINT)DIR::DOWN] = L"BulletKin_Walk_Left";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"BulletKin_Walk_Left";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"BulletKin_Walk_Left";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"BulletKin_Walk_Left";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"BulletKin_Walk_Left";
	}
	break;
	case MONSTER_TYPE::SHOTGUNKIN_BLUE:
	{
		m_strAnimName[(UINT)DIR::UP] = L"ShotgunKinBiue_Walk_Up";
		m_strAnimName[(UINT)DIR::DOWN] = L"ShotgunKinBiue_Walk_Down";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"ShotgunKinBiue_Walk_Left";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"ShotgunKinBiue_Walk_Left";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"ShotgunKinBiue_Walk_Left";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"ShotgunKinBiue_Walk_Left";
	}
	break;
	case MONSTER_TYPE::SHOTGUNKIN_RED:
	{
		m_strAnimName[(UINT)DIR::UP] = L"ShotgunKinRed_Walk_Up";
		m_strAnimName[(UINT)DIR::DOWN] = L"ShotgunKinRed_Walk_Down";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"ShotgunKinRed_Walk_Left";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"ShotgunKinRed_Walk_Left";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"ShotgunKinRed_Walk_Left";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"ShotgunKinRed_Walk_Left";
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
		m_strAnimName[(UINT)DIR::UP] = L"Mage_Attack2_Down_Left";
		m_strAnimName[(UINT)DIR::DOWN] = L"Mage_Attack2_Down_Left";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"Mage_Attack2_Down_Left";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"Mage_Attack2_Down_Left";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"Mage_Attack2_Down_Left";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"Mage_Attack2_Down_Left";
	}
	break;
	}
}

void CMstChase::PlayAnim()
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
