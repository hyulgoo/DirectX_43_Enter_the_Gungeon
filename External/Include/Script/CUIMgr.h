#pragma once
#include <Engine\CSingleton.h>
#include "CHPUIScript.h"
#include "CBossHPScript.h"
#include "CWeaponUIScript.h"
class CUIMgr :
    public CSingleton<CUIMgr>
{
    SINGLE(CUIMgr);

private:
    CHPUIScript*         m_pHPUI;
    CBossHPScript*       m_pBossHPUI;
    CWeaponUIScript*     m_pWeaponUI;

public:
    void RegisterHPUI(CHPUIScript* _pHPUIScript);
    void RegisterBossUI(CBossHPScript* _pBossHPScript);
    void RegisterWeaponUI(CWeaponUIScript* _pWeaponUI);

    void SetHP(int _iHP);
    void SetBlank(int _iBlankCount);
    void SetBullet(int _iBulletCount, bool _bMax);
    void SetWeapon(WEAPON_TYPE _Weapon);
    void PlayWeaponAnim(WEAPON_STATES _tState);
    void BossAppear();
    void SetBossHP(int _iHP);
    void Clear();

    CLONE(CUIMgr);
};

