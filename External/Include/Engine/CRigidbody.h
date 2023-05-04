#pragma once
#include "CComponent.h"

class CRigidbody :
    public CComponent
{
private:
    Vec2    m_vForce;         // ��
    Vec2    m_vVelocity;      // �ӵ�
    float   m_fMass;          // ����

    float   m_fFriction;      // ������
    float   m_fFrictionScale; // ���� ���

    float   m_fVelocityLimit; // ���� �ӵ�

    float   m_fGravityAccel;  // �߷� ���ӵ� ����
    float   m_fGravityVLimit;
    bool    m_bGravityUse;    // �߷� ��뿩��
    bool    m_bGround;        // �� üũ

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

