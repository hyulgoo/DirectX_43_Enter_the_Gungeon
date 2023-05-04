#pragma once
#include "CState.h"
class CBossCharge :
    public CState
{
private:
    float           m_fDelay;
    float           m_fAttackDelay;
    int             m_iBulletCount;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void PlayAnim() override;
    CLONE(CBossCharge);

public:
    CBossCharge();
    ~CBossCharge();
};

