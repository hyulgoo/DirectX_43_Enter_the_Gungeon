#pragma once
#include "CState.h"
class CPlyDeath :
    public CState
{
private:
    float           m_fDelay;
    wstring         m_strAnimName[(UINT)DIR::END];
    CGameObject*    m_pWeapon;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    void SetAnimName(wstring _strUp, wstring _strDown, wstring _strLeftUp, wstring _strLeftDown, wstring _strRightUp, wstring _strRightDown);
    virtual void PlayAnim() override;

    CLONE(CPlyDeath);

public:
    CPlyDeath();
    ~CPlyDeath();
};

