#include "pch.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CBossScript.h"

CMissileScript::CMissileScript()
	: CScript((UINT)SCRIPT_TYPE::MISSILESCRIPT)
	, m_vDir(Vec3(0.f, 0.f, 0.f))
	, m_fSpeed(700.f)
	, m_fDelay(0.f)
	, m_bHangFire(false)
	, m_fAccTime(0.f)
	, m_bRot(false)
	, m_iDamage(0)
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::SetHangFire(Vec3 _vDir, float _vDelay)
{
	m_vDir = _vDir;
	m_fDelay = _vDelay;
	m_bHangFire = true;
}

void CMissileScript::begin()
{
	if(L"Bullet" == GetOwner()->GetName())
		MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"BULLET_NORMAL"));
	else if(L"Crossbow" == GetOwner()->GetName())
		MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"BULLET_CROSSBOW"));

	MeshRender()->GetDynamicMaterial();
	if (m_bRot)
	{
		float fRot = m_fRot - (90.f * XM_PI / 180.f);
		Transform()->SetRelativeRot(0.f, 0.f, fRot);
	}
}

void CMissileScript::tick()
{	
	m_fAccTime += DT;
	SetLifeSpan(2.f);
	Vec3 vPos = Transform()->GetRelativePos();
	vPos += m_vDir * DT * m_fSpeed;
	Transform()->SetRelativePos(vPos);
}

void CMissileScript::BeginOverlap(CCollider2D* _Other)
{
	if((int)LAYER::MONSTER == _Other->GetOwner()->GetLayerIndex())
	{
		if (L"Gatling_Gull" == _Other->GetOwner()->GetName())
		{
			if (_Other->GetOwner()->GetScript<CBossScript>()->IsCollidable())
				_Other->GetOwner()->GetScript<CBossScript>()->SetHitEffect();
			else
				return;
			
		}
		else 
		{
			_Other->GetOwner()->GetScript<CMonsterScript>()->SetHitEffect();
		}
		CGameObject* ShootEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"Bullet_Hit")->Instantiate();
		ShootEffect->MeshRender()->GetDynamicMaterial();
		ShootEffect->SetLifeSpan(0.25f);
		Vec3 CurPos = Transform()->GetWorldPos();
		Vec3 SpawnPos = Vec3(CurPos.x, CurPos.y, CurPos.z - 200.f);
		SpawnGameObject(ShootEffect, SpawnPos, (int)LAYER::MONSTERPROJECTILE);
	}
	else 
	{
		CGameObject* ShootEffect = CResMgr::GetInst()->FindRes<CPrefab>(L"Bullet_Die")->Instantiate();
		ShootEffect->MeshRender()->GetDynamicMaterial();
		ShootEffect->SetLifeSpan(0.3f);
		Vec3 CurPos = Transform()->GetWorldPos();
		Vec3 SpawnPos = Vec3(CurPos.x, CurPos.y, CurPos.z - 200.f);
		SpawnGameObject(ShootEffect, SpawnPos, (int)LAYER::MONSTERPROJECTILE);
	}

	Destroy();
}
