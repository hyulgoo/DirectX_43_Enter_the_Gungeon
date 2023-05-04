#pragma once
#include "CState.h"
class CBossCQC :
    public CState
{
private:
    float   m_fDelay;
    bool    m_bAttack;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void PlayAnim() override;
    CLONE(CBossCQC);

public:
    CBossCQC();
    ~CBossCQC();
};

