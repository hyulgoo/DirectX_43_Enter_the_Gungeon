#pragma once
#include "CState.h"
class CMstFall :
    public CState
{
private:
    wstring         m_strAnimName[(UINT)DIR::END];
    UINT            m_MstType;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    void SetAnimName();
    virtual void PlayAnim() override;
    CLONE(CMstFall);

public:
    CMstFall(UINT _MstType);
    ~CMstFall();
};

