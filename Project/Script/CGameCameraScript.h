#pragma once
#include <Engine/CScript.h>
class CGameCameraScript :
    public CScript
{
private:
    CGameObject* m_Player;
    float        m_fMoveDistance;
    float        m_fMaxLength;
    Vec3         m_vPlayerPos;
    Vec3         m_vDir;
    float        m_Length;

    Vec2		 m_vOffset;
    float		 m_fAccTime;
    float		 m_fMaxTime;
    float		 m_fRange;
    float		 m_fShakeSpeed;
    float		 m_fShakeDir;
    bool		 m_bCamShake;

public:
    virtual void begin() override;
    virtual void tick() override;
    void CameraShake(float _fRange, float _fShackSpeed, float _fTerm);
    void ShackCamera();
private:
    void CameraMove();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CGameCameraScript);
public:
    CGameCameraScript();
    ~CGameCameraScript();
};

