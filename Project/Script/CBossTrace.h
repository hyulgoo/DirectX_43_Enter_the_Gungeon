#pragma once
#include "CState.h"
class CBossTrace :
    public CState
{
private:
    float           m_fATK_Delay;
    float           m_fBreakTime;
    int             m_iBreakCount;
    float           m_tPatternTime;
    float           m_fLength;
    Vec2            m_vBulletSpawnPos;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void PlayAnim() override;
    CLONE(CBossTrace);

public:
    CBossTrace();
    ~CBossTrace();
};

