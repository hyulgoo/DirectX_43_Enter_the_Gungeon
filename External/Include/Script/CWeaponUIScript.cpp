#include "pch.h"
#include "CWeaponUIScript.h"
#include <Engine/CEngine.h>
#include "CUIMgr.h"

CWeaponUIScript::CWeaponUIScript()
	: CScript((UINT)WEAPONUISCRIPT)
	, m_pWeaponFrame(nullptr)
	, m_pWeapon(nullptr)
	, m_strWeaponAnimName{}
	, m_iCurBulletCount(0)
    , m_iMaxBulletCount(0)
{
}

CWeaponUIScript::~CWeaponUIScript()
{

}

void CWeaponUIScript::begin()
{
	if (nullptr == m_pWeaponFrame)
	{
		Vec2 vResolution = CEngine::GetInst()->GetWindowResolution();
		Vec3 vSpawnPos = Vec3(vResolution.x / 2 * 0.8f, -vResolution.y / 2 * 0.8f, 100.f);
		m_pWeaponFrame = CResMgr::GetInst()->FindRes<CPrefab>(L"WeaponFrame")->Instantiate();
		m_pWeaponFrame->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"WEAPONFRAMEUI"));
		SpawnGameObject(m_pWeaponFrame, vSpawnPos, (int)LAYER::UI);
		m_pWeaponFrame->MeshRender()->GetDynamicMaterial();
	}
	if (nullptr == m_pWeapon)
	{
		Vec2 vResolution = CEngine::GetInst()->GetWindowResolution();
		Vec3 vSpawnPos = Vec3(vResolution.x / 2 * 0.78f, -vResolution.y / 2 * 0.8f, 50.f);
		m_pWeapon = CResMgr::GetInst()->FindRes<CPrefab>(L"Weapon")->Instantiate();
		SpawnGameObject(m_pWeapon, vSpawnPos, (int)LAYER::UI);
	}
	CUIMgr::GetInst()->RegisterWeaponUI(this);
}

void CWeaponUIScript::tick()
{
}

void CWeaponUIScript::SetWeaponType(WEAPON_TYPE _type)
{
	WEAPON_TYPE type = _type;
	switch (type)
	{
	case WEAPON_TYPE::OLDGUN:
		m_strWeaponAnimName[(UINT)WEAPON_STATES::IDLE] = L"Oldgun_idle";
		m_strWeaponAnimName[(UINT)WEAPON_STATES::SHOT] = L"Oldgun_fire";
		m_strWeaponAnimName[(UINT)WEAPON_STATES::RELOAD] = L"Oldgun_reload_ing";
		m_iMaxBulletCount = INFINITE;
		break;
	case WEAPON_TYPE::CROSSBOW:
		m_strWeaponAnimName[(UINT)WEAPON_STATES::IDLE] = L"Crossbow_idle";
		m_strWeaponAnimName[(UINT)WEAPON_STATES::SHOT] = L"Crossbow_fire";
		m_strWeaponAnimName[(UINT)WEAPON_STATES::RELOAD] = L"Crossbow_reload";
		m_iMaxBulletCount = 100;
		break;
	case WEAPON_TYPE::MAGNUM:
		m_strWeaponAnimName[(UINT)WEAPON_STATES::IDLE] = L"Magnum_idle";
		m_strWeaponAnimName[(UINT)WEAPON_STATES::SHOT] = L"Magnum_fire";
		m_strWeaponAnimName[(UINT)WEAPON_STATES::RELOAD] = L"Oldgun_reload_ing";
		m_iMaxBulletCount = 100;
		break;
	}
}

void CWeaponUIScript::SetWeaponState(WEAPON_STATES _State)
{
	m_pWeapon->Animator2D()->Play(m_strWeaponAnimName[(UINT)_State], true);
}

void CWeaponUIScript::SetCurBullet(int _iCurBullet)
{
}

void CWeaponUIScript::SetMaxBullet(int _iMaxBullet)
{
}

