#pragma once
#include "CComponent.h"

class CRigidbody :
    public CComponent
{
private:
    Vec2    m_vForce;         // 힘
    Vec2    m_vVelocity;      // 속도
    float   m_fMass;          // 질량

    float   m_fFriction;      // 마찰력
    float   m_fFrictionScale; // 마찰 계수

    float   m_fVelocityLimit; // 제한 속도

    float   m_fGravityAccel;  // 중력 가속도 설정
    float   m_fGravityVLimit;
    bool    m_bGravityUse;    // 중력 사용여부
    bool    m_bGround;        // 땅 체크

public:
    virtual void finaltick() override;

public:
    void AddForce(Vec2 _vForce) { m_vForce += _vForce; }
    void AddVelocity(Vec2 _vVelocity) { m_vVelocity += _vVelocity; }
    void SetVelocityLimit(float _fLimit) { m_fVelocityLimit = _fLimit; }
    void SetGravityVelocityLimit(float _fLimit) { m_fGravityVLimit = _fLimit; }

    void SetVelocity(Vec2 _vVelocity) { m_vVelocity = _vVelocity; }
    Vec2 GetVelocity() { return m_vVelocity; }

    bool IsGround() { return m_bGround; }

    void SetFriction(float _f) { m_fFriction = _f; }
    void SetFrictionScale(float _f) { m_fFrictionScale = _f; }
    void SetGravity(bool _bSet) { m_bGravityUse = _bSet; }
    void SetGravityAccel(float _fAccel) { m_fGravityAccel = _fAccel; }
    void SetGround(bool _bGround);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    CLONE(CRigidbody);

public:
    CRigidbody();
    ~CRigidbody();
};

