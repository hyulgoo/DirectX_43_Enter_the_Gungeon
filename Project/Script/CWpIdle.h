#pragma once
#include "CState.h"
class CWpIdle :
    public CState
{
private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void PlayAnim() override;

    CLONE(CWpIdle);

public:
    CWpIdle();
    ~CWpIdle();
};

