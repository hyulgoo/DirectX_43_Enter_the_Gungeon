#include "pch.h"
#include "CUIMgr.h"

CUIMgr::CUIMgr()
	: m_pHPUI(nullptr)
	, m_pBossHPUI(nullptr)
	, m_pWeaponUI(nullptr)
{
}

CUIMgr::~CUIMgr()
{
}

void CUIMgr::RegisterHPUI(CHPUIScript* _pHPUIScript)
{
	m_pHPUI = _pHPUIScript;
}

void CUIMgr::RegisterBossUI(CBossHPScript* _pBossHPScript)
{
	m_pBossHPUI = _pBossHPScript;
}

void CUIMgr::RegisterWeaponUI(CWeaponUIScript* _pWeaponUI)
{
	m_pWeaponUI = _pWeaponUI;
}

void CUIMgr::SetHP(int _iHP)
{
	if (nullptr == m_pHPUI)
		return;
	m_pHPUI->SetHP(_iHP);
}

void CUIMgr::SetBlank(int _iBlankCount)
{
	if (nullptr == m_pHPUI)
		return;

	m_pHPUI->SetBlank(_iBlankCount);
}

void CUIMgr::SetBullet(int _iBulletCount, bool _bMax)
{
	if (nullptr == m_pWeaponUI)
		return;
	if (_bMax)
		m_pWeaponUI->SetMaxBullet(_iBulletCount);
	else
		m_pWeaponUI->SetCurBullet(_iBulletCount);
}

void CUIMgr::SetWeapon(WEAPON_TYPE _Weapon)
{
	if (nullptr == m_pWeaponUI)
		return;
	m_pWeaponUI->SetWeaponType(_Weapon);
}

void CUIMgr::PlayWeaponAnim(WEAPON_STATES _tState)
{
	if (nullptr == m_pWeaponUI)
		return;
	m_pWeaponUI->SetWeaponState(_tState);
}

void CUIMgr::BossAppear()
{
	if (nullptr == m_pBossHPUI)
		return;
	m_pBossHPUI->BossAppear();
}

void CUIMgr::SetBossHP(int _iHP)
{
	if (nullptr == m_pBossHPUI)
		return;
	m_pBossHPUI->SetBossHP(_iHP);
}

void CUIMgr::Clear()
{
	m_pHPUI = nullptr;
	m_pBossHPUI = nullptr;
	m_pWeaponUI = nullptr;
}