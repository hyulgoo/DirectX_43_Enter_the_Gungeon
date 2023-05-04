#pragma once
#include "CState.h"
class CMstBookAttack :
    public CState
{
private:
    CPrefab*        m_pBullet;
    UINT            m_MstType;
    wstring         m_strAnimName[(UINT)DIR::END];
    float           m_fFirstDelay;
    float           m_fLaterDelay;
    float           m_fAccTime;
    float           m_fSpawnRate;
    int             m_iBulletCount;
    int             m_iMaxBulletCount;
    bool            m_bAttack;
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void PlayAnim() override;
    void SetAnimName();
    void Shoot();
    CLONE(CMstBookAttack);

public:
    CMstBookAttack(UINT _MstType);
    ~CMstBookAttack();
};

