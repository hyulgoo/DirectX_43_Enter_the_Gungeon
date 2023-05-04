#pragma once
#include <Engine\CScript.h>
class CWalkEffectScript :
    public CScript
{
private:
    CGameObject* m_pPlayer;

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CWalkEffectScript);

    CWalkEffectScript();
    CWalkEffectScript(const CWalkEffectScript& _Other);
    ~CWalkEffectScript();
};

