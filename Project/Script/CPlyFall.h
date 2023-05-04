#pragma once
#include "CState.h"
class CPlyFall :
    public CState
{
private:
    float           m_fDelay;
    CGameObject*    m_pWeapon;
    wstring         m_strAnimName[(UINT)DIR::END];
    COLLISION_DIR   m_Dir;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void PlayAnim() override;
    void SetAnimName(wstring _strUp, wstring _strDown, wstring _strLeftUp, wstring _strLeftDown, wstring _strRightUp, wstring _strRightDown);

    CLONE(CPlyFall);

public:
    CPlyFall();
    ~CPlyFall();

    friend class CPlayerScript;
};

