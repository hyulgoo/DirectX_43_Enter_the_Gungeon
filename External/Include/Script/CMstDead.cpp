#include "pch.h"
#include "CMstDead.h"
#include <Engine/CAnim2D.h>
#include "CWaveMgr.h"

CMstDead::CMstDead(UINT _MstType)
	: m_MstType(_MstType)
	, m_strAnimName{}
	, m_bCorpse(false)
	, m_bSound(false)
{
	SetAnimName();
}

CMstDead::~CMstDead()
{
}

void CMstDead::tick()
{
	if (m_bCorpse)
		return;

	if ((UINT)MONSTER_TYPE::GREENBOOK == m_MstType
		|| (UINT)MONSTER_TYPE::REDBOOK == m_MstType
		|| (UINT)MONSTER_TYPE::BULLETBAT == m_MstType
		|| (UINT)MONSTER_TYPE::SHOTBAT == m_MstType)
	{
		if(!m_bSound)
		{
			if ((UINT)MONSTER_TYPE::REDBOOK == m_MstType)
				CResMgr::GetInst()->FindRes<CSound>(L"RedMonsterDie")->Play(1, 1.f, true);
			else if ((UINT)MONSTER_TYPE::GREENBOOK == m_MstType)
				CResMgr::GetInst()->FindRes<CSound>(L"BlueMonsterDie")->Play(1, 1.f, true);
			else
				CResMgr::GetInst()->FindRes<CSound>(L"Death")->Play(1, 0.5f, true);
			m_bSound = true;
		}

		// 시체를 남기지 않는 몬스터는 바로 삭제
		if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish())
		{
			GetOwner()->SetLifeSpan(0.f); 
			CWaveMgr::GetInst()->MinusMstCount();
			m_bCorpse = true;
		}
	}
	else
	{
		if (!m_bSound)
		{
			if((UINT)MONSTER_TYPE::SHOTGUNKIN_BLUE == m_MstType)
				CResMgr::GetInst()->FindRes<CSound>(L"BlueGunDead_Dead")->Play(1, 1.f, true);
			else if ((UINT)MONSTER_TYPE::SHOTGUNKIN_RED == m_MstType)
				CResMgr::GetInst()->FindRes<CSound>(L"RedGunDead_Dead")->Play(1, 1.f, true);
			else
				CResMgr::GetInst()->FindRes<CSound>(L"BulletManDeath")->Play(1, 1.f, true);

			m_bSound = true;
		}
		if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish())
		{
			// 죽으면 알파값 반만 주기
			int a = 1;
			GetOwner()->MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_3, &a);
			Vec3 Pos = GetOwner()->Transform()->GetWorldPos();
			// 시체가 플레이어를 가리지 않게 z축을 플레이어 뒤로 밀어줌.
			GetOwner()->Transform()->SetRelativePos(Pos.x, Pos.y, 550.f);
			// 그림자, 이펙트 등의 Child는 삭제
			vector<CGameObject*> vecChild = GetOwner()->GetChild();
			for (size_t i = 0; i < vecChild.size(); ++i)
			{
				vecChild[i]->SetLifeSpan(0.f);
			}
			m_bCorpse = true;
			CWaveMgr::GetInst()->MinusMstCount();
		}
	}
}

void CMstDead::Enter()
{
	PlayAnim();
	// 필요없어진 충돌체 삭제
	GetOwner()->DeleteComponent((UINT)COMPONENT_TYPE::COLLIDER2D);
}

void CMstDead::Exit()
{
}

void CMstDead::SetAnimName()
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
		m_strAnimName[(UINT)DIR::UP] = L"BulletKin_Death_Up";
		m_strAnimName[(UINT)DIR::DOWN] = L"BulletKin_Death_Down";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"BulletKin_Death_Up_Left";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"BulletKin_Death_Up_Left";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"BulletKin_Death_Up_Left";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"BulletKin_Death_Up_Left";
	}
	break;
	case MONSTER_TYPE::SHOTGUNKIN_BLUE:
	{
		m_strAnimName[(UINT)DIR::UP] = L"ShotgunKinBiue_Death_Right";
		m_strAnimName[(UINT)DIR::DOWN] = L"ShotgunKinBiue_Death_Right";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"ShotgunKinBiue_Death_Left";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"ShotgunKinBiue_Death_Left";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"ShotgunKinBiue_Death_Left";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"ShotgunKinBiue_Death_Left";
	}
	break;
	case MONSTER_TYPE::SHOTGUNKIN_RED:
	{
		m_strAnimName[(UINT)DIR::UP] = L"ShotgunKinRed_Death_Left";
		m_strAnimName[(UINT)DIR::DOWN] = L"ShotgunKinRed_Death_LeftDown";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"ShotgunKinRed_Death_Left";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"ShotgunKinRed_Death_LeftDown";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"ShotgunKinRed_Death_Left";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"ShotgunKinRed_Death_LeftDown";
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

void CMstDead::PlayAnim()
{
	CAnimator2D* Anim = GetOwner()->Animator2D();
	switch (GetDir())
	{
	case DIR::UP:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::UP], false);
		break;
	case DIR::DOWN:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::DOWN], false);
		break;
	case DIR::LEFT_UP:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::LEFT_UP], false);
		break;
	case DIR::LEFT_DOWN:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::LEFT_DOWN], false);
		break;
	case DIR::RIGHT_UP:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::RIGHT_UP], false);
		break;
	case DIR::RIGHT_DOWN:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::RIGHT_DOWN], false);
		break;
	}
}
