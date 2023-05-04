#pragma once
#include "CState.h"
class CMstKinAttack :
    public CState
{
private:
    UINT            m_MstType;
    CGameObject*    m_pWeapon;
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void PlayAnim() override;
    void Shoot();
    CLONE(CMstKinAttack);

public:
    CMstKinAttack(UINT _MstType);
    CMstKinAttack(const CMstKinAttack& _Other);
    ~CMstKinAttack();
};

