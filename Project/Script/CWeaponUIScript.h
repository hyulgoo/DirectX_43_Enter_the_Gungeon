#pragma once
#include <Engine\CScript.h>

class CWeaponUIScript :
    public CScript
{
private:
    CGameObject* m_pWeaponFrame;
    CGameObject* m_pWeapon;
    wstring      m_strWeaponAnimName[(UINT)WEAPON_STATES::END];
    int          m_iCurBulletCount;
    int          m_iMaxBulletCount;
public:
    virtual void begin() override;
    virtual void tick() override;

    void SetWeaponType(WEAPON_TYPE _type);
    void SetWeaponState(WEAPON_STATES _State);
    void SetCurBullet(int _iCurBullet);
    void SetMaxBullet(int _iMaxBullet);

    CLONE(CWeaponUIScript);

    CWeaponUIScript();
    ~CWeaponUIScript();
};

