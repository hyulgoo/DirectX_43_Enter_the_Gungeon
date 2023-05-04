#pragma once
#include "CState.h"
class CMstDead :
    public CState
{
private:
    wstring         m_strAnimName[(UINT)DIR::END];
    UINT            m_MstType;
    bool            m_bCorpse;
    bool            m_bSound;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    void SetAnimName();
    virtual void PlayAnim() override;
    CLONE(CMstDead);

public:
    CMstDead(UINT _MstType);
    ~CMstDead();
};

