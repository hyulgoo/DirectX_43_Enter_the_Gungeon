#pragma once
#include "CState.h"
class CPlyKick :
    public CState
{
private:
    float           m_fDelay;
    wstring         m_strAnimName[(UINT)DIR::END];

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void PlayAnim() override;
    void SetAnimName(wstring _strUp, wstring _strDown, wstring _strLeftUp, wstring _strLeftDown, wstring _strRightUp, wstring _strRightDown);

    CLONE(CPlyKick);

public:
    CPlyKick();
    ~CPlyKick();
};

