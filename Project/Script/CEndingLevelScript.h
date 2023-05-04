#pragma once
#include <Engine\CScript.h>
class CEndingLevelScript :
    public CScript
{
private:
    CGameObject* m_pEngineScreen;

public:
    virtual void begin();
    virtual void tick();

    CLONE(CEndingLevelScript);

    CEndingLevelScript();
    ~CEndingLevelScript();
};

