#pragma once
#include "CComponent.h"
class CTransform :
    public CComponent
{
private:
    Vec3    m_vRelativePos;
    Vec3    m_vRelativeScale;
    Vec3    m_vRelativeRot;
    Vec3    m_vPrevPos;

    bool    m_bAbsolute;        // ��� �̵�, ũ�⸦ ���밪����

    Vec3    m_vRelativeDir[3];  // �θ���� ���Ե� ���� ����
    Vec3    m_vWorldDir[3];     // ���� ����

    Matrix  m_matWorldScale; // ���� �θ���� ������ ũ�Ⱚ
    Matrix  m_matWorld; // ũ��, ȸ��, �̵� ������ ���ĳ��� WorldView

public:
    void SetRelativePos(Vec3 _vPos) { m_vRelativePos = _vPos; }
    void SetRelativeScale(Vec3 _vScale) { m_vRelativeScale = _vScale; }
    void SetRelativeRot(Vec3 _vRot) { m_vRelativeRot = _vRot; }

    void SetRelativePos(float _x, float _y, float _z) { m_vRelativePos = Vec3(_x, _y, _z); }
    void SetRelativeScale(float _x, float _y, float _z) { m_vRelativeScale = Vec3(_x, _y, _z); }
    void SetRelativeRot(float _x, float _y, float _z) { m_vRelativeRot = Vec3(_x, _y, _z); }

    void SetAbsolute(bool _Set) { m_bAbsolute = _Set; }

    Vec3 GetRelativePos() const { return m_vRelativePos; }
    Vec3 GetRelativeScale() const { return m_vRelativeScale; }
    Vec3 GetRelativeRot() const { return m_vRelativeRot; }

    Vec3 GetRelativeDir(DIR_TYPE _type) const { return m_vRelativeDir[(UINT)_type]; }
    Vec3 GetWorldDir(DIR_TYPE _type) const { return m_vWorldDir[(UINT)_type]; }
    // ��ġ���� �ش��ϴ� 4�� 1, 2, 3�� �߶� ��������.
    Vec3 GetWorldPos() { return m_matWorld.Translation(); }

    Vec3 GetPrevPos() { return m_vPrevPos; }
    Vec3 GetFrmMove() { return m_vPrevPos - m_matWorld.Translation(); }

    const Matrix& GetWorldScaleMat() const { return m_matWorldScale; }
    const Matrix& GetWorldMat() const { return m_matWorld; }

    void SetWorldMat(const Matrix& _mat) { m_matWorld = _mat; }

public:
    virtual void finaltick() override;
    void UpdateData();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CTransform);

public:
    CTransform();
    ~CTransform();
};
