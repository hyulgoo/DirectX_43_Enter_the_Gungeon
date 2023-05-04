#pragma once
#include "CState.h"
class CBossIdle :
    public CState
{
private:
    bool    m_bShout;
    float   m_fDelay;
    bool    m_bShowIntro;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    CLONE(CBossIdle);

public:
    CBossIdle();
    ~CBossIdle();
};

