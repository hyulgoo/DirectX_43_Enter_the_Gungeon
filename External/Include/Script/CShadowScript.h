#pragma once
#include <Engine\CScript.h>
class CShadowScript :
    public CScript
{
private:
    CGameObject* m_pTarget;
    float       m_fYPos;

public:
    virtual void begin() override;
    virtual void tick() override;
    void SetFloorHeight(float _Height) { m_fYPos = _Height; }

    CLONE(CShadowScript);

    CShadowScript();
    ~CShadowScript();
};

