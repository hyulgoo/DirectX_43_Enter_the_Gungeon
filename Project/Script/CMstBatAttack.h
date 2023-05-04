#pragma once
#include "CState.h"
class CMstBatAttack :
    public CState
{
private:
    wstring         m_strAnimName;
    UINT            m_MstType;
    Vec2            m_vDir;
    float           m_fDelay;
    float           m_fAccTime;
    int             m_iBulletCount;
    bool            m_bAttack;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    void SetAnimName();

    virtual void BeginOverlap(CCollider2D* _Other);
    CLONE(CMstBatAttack);

public:
    CMstBatAttack(UINT _MstType);
    ~CMstBatAttack();
};

