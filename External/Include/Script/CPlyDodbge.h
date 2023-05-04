#pragma once
#include "CState.h"
class CPlyDodbge :
    public CState
{
private:
    CGameObject*    m_pWalkEffect;
    CGameObject*    m_pWeapon;
    float           m_fDelay;
    wstring         m_strAnimName[(UINT)DIR::END];
    Vec2            m_vDir;
    float           m_fSpeed;
    DIR             m_DIR;
    COLLISION_DIR   m_CollisionDir;
    bool            m_bCollision;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void PlayAnim() override;
    void CheckCollision(COLLISION_DIR _Dir);
    void SetAnimName(wstring _strUp, wstring _strDown, wstring _strLeftUp, wstring _strLeftDown, wstring _strRightUp, wstring _strRightDown);

    virtual void BeginOverlap(CCollider2D* _Other);
    virtual void EndOverlap(CCollider2D* _Other);

    CLONE(CPlyDodbge);

public:
    CPlyDodbge();
    ~CPlyDodbge();
};

