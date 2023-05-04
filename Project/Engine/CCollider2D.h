#pragma once
#include "CComponent.h"
class CCollider2D :
    public CComponent
{
private:
    Vec3            m_vOffsetPos;
    Vec3            m_vOffsetScale;
    bool            m_bAbsolute;
    COLLIDER2D_TYPE m_Shape;

    Matrix          m_matCollider2D;  // Coliider의 월드행렬

    int             m_iCollisionCount;

    Vec2            m_CollisionArea;
    COLLISION_DIR   m_Dir;
    bool            m_bPush;
           
    
private:
    void SetCollisionArea(Vec2 _CollisionArea) { m_CollisionArea = _CollisionArea; }

public:
    virtual void finaltick() override;

public:
    void SetOffsetPos(Vec2 _vOffsetPos) { m_vOffsetPos = Vec3(_vOffsetPos.x, _vOffsetPos.y, 0.f); }
    void SetOffsetScale(Vec2 _vOffsetScale) { m_vOffsetScale = Vec3(_vOffsetScale.x, _vOffsetScale.y, 1.f); }
    void SetAbsolute(bool _bSet) { m_bAbsolute = _bSet; }
    void SetCollider2DType(COLLIDER2D_TYPE _Type) { m_Shape = _Type; }
    void SetPush(bool _bPush) { m_bPush = _bPush; }
    bool IsPushable() { return m_bPush; }
    Vec2 GetCollisionArea() { return m_CollisionArea; }

    Vec3 GetOffsetScale() { return m_vOffsetScale; }
    Vec3 GetOffsetPos() { return m_vOffsetPos; }

    COLLIDER2D_TYPE GetCollider2DType() { return m_Shape; }
    int GetCollisionCount() { return m_iCollisionCount; }
    bool IsAbsolute() { return m_bAbsolute; }
    const Matrix& GetColliderWorldMat() { return m_matCollider2D; }

    COLLISION_DIR GetDir() { return m_Dir; }
    void SetDir(COLLISION_DIR _Dir) { m_Dir = _Dir; }
    

public:
    void BeginOverlap(CCollider2D* _Other);
    void OnOverlap(CCollider2D* _Other);
    void EndOverlap(CCollider2D* _Other);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CCollider2D);
public:
    CCollider2D();
    ~CCollider2D();

    friend class CCollisionMgr;
};

