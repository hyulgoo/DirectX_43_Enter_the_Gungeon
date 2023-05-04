#pragma once
#include <Engine\CScript.h>
class CMstGunScript :
    public CScript
{
private:
    CStateScript*   m_pState;
    float           m_fAttackDelay;
    Vec3            m_vPlayerPos;
    Vec2            m_vDirtoPly;
    float           m_fDirtiPly;



public:

    virtual void begin() override;
    virtual void tick() override;

    void SetRelativePos();
    void SetRelativeRot();
    void Shoot();

    CLONE(CMstGunScript);
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CMstGunScript();
    CMstGunScript(const CMstGunScript& _Other);
    ~CMstGunScript();
};

