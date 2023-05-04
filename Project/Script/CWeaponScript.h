#pragma once
#include <Engine/CScript.h>
class CStateScript;

class CWeaponScript :
    public CScript
{
private:
    CStateScript*   m_pState;
    CGameObject*    m_pFireEffect;
    CGameObject*    m_pLightEffect;
    WEAPON_TYPE     m_Type;
    Vec3            m_ParentPos;
    float           m_fDir;
    float           m_AttackDelay;
    int             m_CurBulletCount;
    int             m_MaxBulletCount;
    int             m_iMOA;
    float           m_BulletDir;

public:
    virtual void begin() override;
    virtual void tick() override;

    CGameObject* GetLightEffect() { return m_pLightEffect; }
    void SetRelativePos();
    void SetRelativeRot();
    void ChangeWeapon(WEAPON_TYPE _Weapon);
    WEAPON_TYPE GetWeaponType() { return m_Type; }
    void Shoot();
    void Reload();


    CLONE(CWeaponScript);
public:
    CWeaponScript();
    ~CWeaponScript();
};

