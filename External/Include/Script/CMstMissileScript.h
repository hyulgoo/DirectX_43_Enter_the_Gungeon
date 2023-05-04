#pragma once
#include <Engine\CScript.h>
class CMstMissileScript :
    public CScript
{
private:
    Vec3    m_vDir;
    float   m_fSpeed;
    float   m_fDelay;
    float   m_fAccTime;
    bool    m_bHangFire;
    bool    m_bGuided;

public:
    void SetSpeed(float _Speed) { m_fSpeed = _Speed; }
    void SetDir(Vec3 _vDir) { m_vDir = Vec3(_vDir.x, _vDir.y, 0.f).Normalize(); }
    void SetDir(Vec2 _vDir) { m_vDir = Vec3(_vDir.x, _vDir.y, 0.f).Normalize(); }
    void SetHangFire(Vec3 _vDir, float _vDelay);
    void SetGuided(float _vDelay);

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    //virtual void OnOverlap(CCollider2D* _Other) override;
    //virtual void EndOverlap(CCollider2D* _Other) override;

    CLONE(CMstMissileScript);

public:
    CMstMissileScript();
    ~CMstMissileScript();
};

