#include "pch.h"
#include "CMstMissileScript.h"
#include "CPlayerScript.h"
#include "CStateScript.h"
#include "CUIMgr.h"

CMstMissileScript::CMstMissileScript()
	: CScript((UINT)SCRIPT_TYPE::MSTMISSILESCRIPT)
	, m_vDir(Vec3(0.f, 0.f, 0.f))
	, m_fSpeed(700.f)
	, m_fDelay(0.f)
	, m_fAccTime(0.f)
	, m_bHangFire(false)
	, m_bGuided(false)
{
}

CMstMissileScript::~CMstMissileScript()
{
}

void CMstMissileScript::SetHangFire(Vec3 _vDir, float _vDelay)
{
	m_vDir = _vDir;
	m_fDelay = _vDelay;
	m_bHangFire = true;
}

void CMstMissileScript::SetGuided(float _vDelay)
{
	m_fDelay = _vDelay;
	m_bGuided = true;
}

void CMstMissileScript::begin()
{
	MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"BULLET_MONSTER"));
	MeshRender()->GetDynamicMaterial();
}

void CMstMissileScript::tick()
{
	if (m_bHangFire)
	{
		if (0 < m_fDelay)
		{
			m_fDelay -= DT;
		}
		else 
			m_bHangFire = false;
	}
	else if(m_bGuided)
	{
		if (0 < m_fDelay)
		{
			m_fDelay -= DT;
		}
		else 
		{
			Vec3 CurPos = Transform()->GetWorldPos();
			Vec3 PlyPos = CLevelMgr::GetInst()->FindObjectByName(L"Player")->Transform()->GetWorldPos();
			Vec3 vDir = Vec3(PlyPos.x - CurPos.x, PlyPos.y - CurPos.y, 0.f);
			vDir.Normalize();
			m_vDir = vDir;
			m_bGuided = false;
		}
	}
	else
	{
		m_fAccTime += DT;
		SetLifeSpan(2.f);
		Vec3 vPos = Transform()->GetRelativePos();
		vPos += m_vDir * DT * m_fSpeed;
		Transform()->SetRelativePos(vPos);
	}
}

void CMstMissileScript::BeginOverlap(CCollider2D* _Other)
{
	if((int)LAYER::PLAYER == _Other->GetOwner()->GetLayerIndex())
	{
		bool Able = _Other->GetOwner()->GetScript<CPlayerScript>()->IsInvincible();
		if (Able)
			return;
		_Other->GetOwner()->GetScript<CPlayerScript>()->SetHitEffect();
		CGameObject* ShootEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"Bullet_Hit")->Instantiate();
		ShootEffect->MeshRender()->GetDynamicMaterial();
		ShootEffect->SetLifeSpan(0.25f);
		Vec3 CurPos = Transform()->GetWorldPos();
		Vec3 SpawnPos = Vec3(CurPos.x, CurPos.y, 250.f);
		SpawnGameObject(ShootEffect, SpawnPos, (int)LAYER::MONSTERPROJECTILE);
		CResMgr::GetInst()->FindRes<CSound>(L"Attacked")->Play(1);
		Destroy();
	}
	if ((int)LAYER::WALL == _Other->GetOwner()->GetLayerIndex())
	{
		CGameObject* ShootEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"Bullet_Die")->Instantiate();
		ShootEffect->MeshRender()->GetDynamicMaterial();
		ShootEffect->SetLifeSpan(0.3f);
		Vec3 CurPos = Transform()->GetWorldPos();
		Vec3 SpawnPos = Vec3(CurPos.x, CurPos.y, 250.f);
		SpawnGameObject(ShootEffect, SpawnPos, (int)LAYER::MONSTERPROJECTILE);
		Destroy();
	}

	if ((int)LAYER::BLANK == _Other->GetOwner()->GetLayerIndex())
	{
		CGameObject* ShootEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"Bullet_Hit")->Instantiate();
		ShootEffect->MeshRender()->GetDynamicMaterial();
		ShootEffect->SetLifeSpan(0.25f);
		Vec3 CurPos = Transform()->GetWorldPos();
		Vec3 SpawnPos = Vec3(CurPos.x, CurPos.y, 250.f);
		SpawnGameObject(ShootEffect, SpawnPos, (int)LAYER::MONSTERPROJECTILE);
		Destroy();
	}

}
