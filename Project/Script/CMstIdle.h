#pragma once
#include "CState.h"
class CMstIdle :
    public CState
{
private:
    wstring         m_strAnimName[(UINT)DIR::END];
    UINT            m_MstType;
    float           m_DetectRange;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    void SetAnimName();
    virtual void PlayAnim() override;
    CLONE(CMstIdle);

public:
    CMstIdle(UINT _MstType);
    ~CMstIdle();
};

