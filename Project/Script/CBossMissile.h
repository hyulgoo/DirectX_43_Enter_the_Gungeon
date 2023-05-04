#pragma once
#include "CState.h"
class CBossMissile :
    public CState
{
private:
    int           m_iMissileCount;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void PlayAnim() override;
    CLONE(CBossMissile);

public:
    CBossMissile();
    ~CBossMissile();
};

