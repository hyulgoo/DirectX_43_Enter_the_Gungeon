#pragma once
#include <Engine\CScript.h>
class CFireLightScript :
    public CScript
{
private:
    CGameObject*    m_pParent;
    Vec3            m_vParentPos;
    bool            m_bSetLight;
    float           m_fDelay;

public:
    virtual void begin() override;
    virtual void tick() override;
    void SetPos();
    void SetLightAngle(float _fAngle);
    void SetLightDiffuse(float _fDiffuse);
    void SetLight(bool _bLight);

    CLONE(CFireLightScript);

    CFireLightScript();
    ~CFireLightScript();
};

