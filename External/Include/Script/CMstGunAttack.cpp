#include "pch.h"
#include "CMstGunAttack.h"
#include <Engine/CAnim2D.h>
#include "CMstMissileScript.h"
#include "CMonsterScript.h"
#include "CStateScript.h"
#include "CFireLightScript.h"

CMstGunAttack::CMstGunAttack(UINT _MstType)
	: m_pBullet(nullptr)
	, m_MstType(_MstType)
	, m_fDelay(0.f)
	, m_iBulletCount(0)
	, m_iMOA(0)
	, m_bBarrage(false)
{ 
}

CMstGunAttack::~CMstGunAttack()
{
}
void CMstGunAttack::tick()
{
	m_fDelay -= DT;
	if(0 >= m_fDelay)
	{
		for (int i = 0; i < m_iBulletCount; ++i)
		{
			float RandomAngle = (float)((rand() * 33) % m_iMOA);
			RandomAngle /= 57.3f;
			CGameObject* pBullet = m_pBullet->Instantiate();
			Vec2 vDirtoPly = GetOwner()->GetParent()->GetScript<CMonsterScript>()->GetDirtoPly();
			vDirtoPly.Normalize();
			float x = acos(vDirtoPly.x);
			float y = asin(vDirtoPly.y);
			x += RandomAngle - m_iMOA * 0.5f / 57.3f;
			y += RandomAngle - m_iMOA * 0.5f / 57.3f;
			Vec2 vDir = Vec2(cos(x), sin(y));
			pBullet->GetScript<CMstMissileScript>()->SetDir(Vec3(vDir.x, vDir.y, 0.f));
			Vec3 CurPos = GetOwner()->Transform()->GetWorldPos();
			SpawnGameObject(pBullet, CurPos, (int)LAYER::MONSTERPROJECTILE);
			if (i == m_iBulletCount - 1)
			{
				if (m_bBarrage)
				{
					m_fDelay = 0.6f;
					PlayAnim();
					m_bBarrage = false;
				}
				else
					m_fDelay = 15.f;
			}
		}
	}

	if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish())
		ChangeState(L"Idle");
}

void CMstGunAttack::Enter()
{
	if (nullptr == m_pBullet)
		m_pBullet = CResMgr::GetInst()->FindRes<CPrefab>(L"MstBullet").Get();
	
	switch ((MONSTER_TYPE)m_MstType)
	{
	case MONSTER_TYPE::BULLETKIN:
	{
		m_iBulletCount = 1;
		m_iMOA = 5;
		m_fDelay = 1.f;
	}
		break;
	case MONSTER_TYPE::SHOTGUNKIN_BLUE:
	{
		m_iBulletCount = 3;
		m_iMOA = 25;
		m_fDelay = 0.66f;
		m_bBarrage = true;
	}
		break;
	case MONSTER_TYPE::SHOTGUNKIN_RED:
	{
		m_iBulletCount = 5;
		m_iMOA = 40;
		m_fDelay = 0.66f;
	}
		break;
	}

	PlayAnim();
}

void CMstGunAttack::Exit()
{
	GetOwner()->Animator2D()->GetCurAnim()->Reset();
	GetOwner()->GetParent()->GetScript<CStateScript>()->ChangeState(L"Idle");
}

void CMstGunAttack::PlayAnim()
{
	switch ((MONSTER_TYPE)GetOwner()->GetParent()->GetScript<CMonsterScript>()->GetType())
	{
	case MONSTER_TYPE::BULLETKIN:
		GetOwner()->Animator2D()->Play(L"Magnum_fire", false);
		CResMgr::GetInst()->FindRes<CSound>(L"Pistol")->Play(1, 0.5f, true);
		break;
	case MONSTER_TYPE::SHOTGUNKIN_BLUE:
		GetOwner()->Animator2D()->Play(L"Shotgun_fire", false);
		CResMgr::GetInst()->FindRes<CSound>(L"ShotGun_Shot")->Play(1, 0.5f, true);
		break;
	case MONSTER_TYPE::SHOTGUNKIN_RED:
		GetOwner()->Animator2D()->Play(L"Shotgun_fire", false);
		CResMgr::GetInst()->FindRes<CSound>(L"ShotGun_Shot")->Play(1, 0.5f, true);
		break;
	}
}
