#pragma once
#include "CState.h"
class CMstGunIdle :
    public CState
{
private:
    UINT    m_Type;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void PlayAnim() override;

    CLONE(CMstGunIdle);

public:
    CMstGunIdle(UINT _MstType);
    ~CMstGunIdle();
};

