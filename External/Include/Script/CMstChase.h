#pragma once
#include "CState.h"
class CMstChase :
    public CState
{
private:
    wstring         m_strAnimName[(UINT)DIR::END];
    UINT            m_MstType;
    float           m_AttackRange;
    float           m_DetectRange; 

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;
    void SetAnimName();
    virtual void PlayAnim() override;
    CLONE(CMstChase);

public:
    CMstChase(UINT _MstType);
    ~CMstChase();
};

