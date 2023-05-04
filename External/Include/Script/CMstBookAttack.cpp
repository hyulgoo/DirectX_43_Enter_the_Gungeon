#include "pch.h"
#include "CMstBookAttack.h"
#include <Engine/CAnim2D.h>
#include "CMstMissileScript.h"
#include "CMonsterScript.h"

CMstBookAttack::CMstBookAttack(UINT _MstType)
	: m_MstType(_MstType)
	 , m_pBullet(nullptr)
	 , m_strAnimName{}
     , m_fLaterDelay(0.f)
     , m_fAccTime(0.f)
	 , m_fSpawnRate(0.f)
     , m_iBulletCount(0)
     , m_bAttack(false)
{
	SetAnimName();
}

CMstBookAttack::~CMstBookAttack()
{
}

void CMstBookAttack::tick()
{
	// 공격을 하지 않았을 때
	if (!m_bAttack)
	{
		if (0 <= m_fFirstDelay)
		{
			// 선딜 감소
			m_fFirstDelay -= DT;
		}
		else
		{
			if (0 <= m_fLaterDelay)
				Shoot();
			else
			{
				m_bAttack = true;
			}
		}
	}
	if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish())
			ChangeState(L"Idle");
}

void CMstBookAttack::Enter()
{

	CResMgr::GetInst()->FindRes<CSound>(L"BookCharge")->Play(1, 1, true);
	if (nullptr == m_pBullet)
	{
		m_pBullet = CResMgr::GetInst()->FindRes<CPrefab>(L"MstBullet").Get();
	}
	tStatus Stat = GetOwnerScript()->GetStat();
	m_fFirstDelay = Stat.Attack_FirstDelay;
	m_fLaterDelay = Stat.Attack_LaterDelay;

	PlayAnim();
}

void CMstBookAttack::Exit()
{
	GetOwner()->Animator2D()->GetCurAnim()->Reset();
	tStatus Stat = GetOwnerScript()->GetStat();
	m_fFirstDelay = Stat.Attack_FirstDelay;
	m_fLaterDelay = Stat.Attack_LaterDelay;
	m_bAttack = false;
	m_iBulletCount = 0;
	m_fAccTime = 0.f;
}

void CMstBookAttack::Shoot()
{
	if (m_iBulletCount == 0)
		CResMgr::GetInst()->FindRes<CSound>(L"BookAttack")->Play(1);
	vector<Vec3> vecBulletPos = {};
	UINT Slice = m_iMaxBulletCount;
	float fRadius = 250.f;
	float fTheta = XM_2PI / (float)m_iMaxBulletCount;
	for (int i = 0; i < m_iMaxBulletCount; ++i)
	{
		Vec3 BulletPos = Vec3(fRadius * cosf(fTheta * (float)i), fRadius * sinf(fTheta * (float)i), 1.f);
		vecBulletPos.push_back(BulletPos);
	}
	m_fLaterDelay -= DT;
	m_fAccTime += DT;
	if (m_fAccTime >= m_fSpawnRate)
	{
		if (m_iMaxBulletCount == m_iBulletCount)
			return;
		m_iBulletCount += 1;
		m_fAccTime -= 0.05f;
		Vec3 vDir = vecBulletPos[m_iBulletCount - 1].Normalize();
		CGameObject* Bullet = m_pBullet->Instantiate();
		SpawnGameObject(Bullet, GetOwner()->Transform()->GetWorldPos() + vDir * 50.f, (int)LAYER::MONSTERPROJECTILE);
		if((UINT)MONSTER_TYPE::REDBOOK == m_MstType)
		{
			Bullet->GetScript<CMstMissileScript>()->SetGuided(1.f);
		}
		else
		{
			Bullet->GetScript<CMstMissileScript>()->SetHangFire(vDir
				, (m_iMaxBulletCount * m_fSpawnRate) - (m_iBulletCount * m_fSpawnRate));
		}

	}
}

void CMstBookAttack::PlayAnim()
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

void CMstBookAttack::SetAnimName()
{
	switch ((MONSTER_TYPE)m_MstType)
	{
	case MONSTER_TYPE::REDBOOK:
	{		
		m_strAnimName[(UINT)DIR::UP] = L"RedBook_Attack";
		m_strAnimName[(UINT)DIR::DOWN] = L"RedBook_Attack";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"RedBook_Attack";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"RedBook_Attack";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"RedBook_Attack";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"RedBook_Attack";
		m_iMaxBulletCount = 15;
		m_fSpawnRate = 0.05f;
	}
	break;
	case MONSTER_TYPE::GREENBOOK:
	{
		m_strAnimName[(UINT)DIR::UP] = L"GreenBook_Attack";
		m_strAnimName[(UINT)DIR::DOWN] = L"GreenBook_Attack";
		m_strAnimName[(UINT)DIR::LEFT_UP] = L"GreenBook_Attack";
		m_strAnimName[(UINT)DIR::LEFT_DOWN] = L"GreenBook_Attack";
		m_strAnimName[(UINT)DIR::RIGHT_UP] = L"GreenBook_Attack";
		m_strAnimName[(UINT)DIR::RIGHT_DOWN] = L"GreenBook_Attack";
		m_iMaxBulletCount = 20;
		m_fSpawnRate = 0.05f;
	}
	break;
	}
}
