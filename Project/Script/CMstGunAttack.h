#pragma once
#include "CState.h"
class CMstGunAttack :
    public CState
{
private:
    CPrefab*   m_pBullet;
    UINT       m_MstType;
    float      m_fDelay;
    int        m_iBulletCount;
    int        m_iMOA;
    bool       m_bBarrage;
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void PlayAnim() override;


    CLONE(CMstGunAttack);

public:
    CMstGunAttack(UINT _MstType);
    ~CMstGunAttack();
};

