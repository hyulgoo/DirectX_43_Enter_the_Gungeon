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

    bool    m_bAbsolute;        // 상대 이동, 크기를 절대값으로

    Vec3    m_vRelativeDir[3];  // 부모까지 포함된 월드 방향
    Vec3    m_vWorldDir[3];     // 로컬 방향

    Matrix  m_matWorldScale; // 제일 부모부터 누적된 크기값
    Matrix  m_matWorld; // 크기, 회전, 이동 정보를 합쳐놓은 WorldView

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
    // 위치값에 해당하는 4열 1, 2, 3만 잘라서 전달해줌.
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

