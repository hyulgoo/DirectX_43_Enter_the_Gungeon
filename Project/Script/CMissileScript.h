#pragma once
#include <Engine\CScript.h>
class CMissileScript :
    public CScript
{
private:
    Vec3    m_vDir;
    float   m_fSpeed;
    float   m_fDelay;
    bool    m_bHangFire;
    float   m_fAccTime;
    float   m_fRot;
    bool    m_bRot;
    int   m_iDamage;

public:
    void SetSpeed(float _Speed) { m_fSpeed = _Speed; }
    void SetDir(Vec3 _vDir) { m_vDir = Vec3(_vDir.x, _vDir.y, 0.f).Normalize(); }
    void SetDir(Vec2 _vDir) { m_vDir = Vec3(_vDir.x, _vDir.y, 0.f).Normalize(); }
    void SetHangFire(Vec3 _vDir, float _vDelay);
    void SetRot(float _fRot) { m_bRot = true; m_fRot = _fRot; }
    void SetDamage(int _iDamage) { m_iDamage = _iDamage; }
    int GetDamage() { return m_iDamage; }

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
   //virtual void OnOverlap(CCollider2D* _Other) override;
   //virtual void EndOverlap(CCollider2D* _Other) override;

    CLONE(CMissileScript);

public:
    CMissileScript();
    ~CMissileScript();
};

