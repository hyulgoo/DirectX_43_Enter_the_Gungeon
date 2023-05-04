#pragma once
#include "CState.h"

enum class JUMP_PATTERN
{
    UP,
    WARNING,
    DOWN,
};

class CBossJump :
    public CState
{
private:
    JUMP_PATTERN    m_ATKState;
    Vec2            m_vDir;
    float           m_fDelay;
    bool            m_bSpawnEffect;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void PlayAnim() override;
    CLONE(CBossJump);

public:
    CBossJump();
    ~CBossJump();
};

