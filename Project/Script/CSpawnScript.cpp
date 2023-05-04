#include "pch.h"
#include "CSpawnScript.h"

CSpawnScript::CSpawnScript()
	: CScript((UINT)SCRIPT_TYPE::SPAWNSCRIPT)
	, m_strSpawnMstName{}
	, m_iSpawnMstNum(-1)
	, m_fDelay(0.f)
	, m_bSpawn(false)
{
}

CSpawnScript::~CSpawnScript()
{
}

void CSpawnScript::begin()
{
	CResMgr::GetInst()->FindRes<CSound>(L"Spawning")->Play(1, 0.5f, true);
}

void CSpawnScript::tick()
{
	if(Animator2D()->IsFinish())
	{	
		if (GetOwner()->IsDead())
			return;
		switch (m_iSpawnMstNum)
		{
		case (int)MONSTER_TYPE::REDBOOK:
			m_strSpawnMstName = L"RedBook";
			break;
		case (int)MONSTER_TYPE::GREENBOOK:
			m_strSpawnMstName = L"GreenBook";
			break;
		case (int)MONSTER_TYPE::BULLETKIN:
			m_strSpawnMstName = L"BulletKin";
			break;
		case (int)MONSTER_TYPE::SHOTGUNKIN_BLUE:
			m_strSpawnMstName = L"ShotgunKin_Blue";
			break;
		case (int)MONSTER_TYPE::SHOTGUNKIN_RED:
			m_strSpawnMstName = L"ShotgunKin_Red";
			break;
		case (int)MONSTER_TYPE::SHOTBAT:
			m_strSpawnMstName = L"ShotBat";
			break;
		case (int)MONSTER_TYPE::BULLETBAT:
			m_strSpawnMstName = L"BulletBat";
			break;
		case (int)MONSTER_TYPE::GATLING_GULL:
			m_strSpawnMstName = L"Gatling_Gull";
			break;
		}
		CGameObject* pMst = CResMgr::GetInst()->FindRes<CPrefab>(m_strSpawnMstName)->Instantiate();
		SpawnGameObject(pMst, Transform()->GetWorldPos(), (int)LAYER::MONSTER);
		DestroyObject(GetOwner());
		m_bSpawn = true;
		CResMgr::GetInst()->FindRes<CSound>(L"SpawnEnd")->Play(1, 0.7f, true);
		GetOwner()->SetLifeSpan(0.f);
	}
}
