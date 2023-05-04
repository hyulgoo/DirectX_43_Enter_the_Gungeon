#pragma once
#include "CState.h"
class CWpReload :
    public CState
{
private:
    float           m_fDelay;
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void PlayAnim() override;

    CLONE(CWpReload);

public:
    CWpReload();
    ~CWpReload();
};

