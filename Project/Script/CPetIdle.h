#pragma once
#include "CState.h"
class CPetIdle :
    public CState
{
private:
    wstring         m_strAnimName[(UINT)DIR::END];

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    void SetAnimName(wstring _strUp, wstring _strDown, wstring _strLeftUp, wstring _strLeftDown, wstring _strRightUp, wstring _strRightDown);
    virtual void PlayAnim() override;

    CLONE(CPetIdle);

public:
    CPetIdle();
    ~CPetIdle();
};

