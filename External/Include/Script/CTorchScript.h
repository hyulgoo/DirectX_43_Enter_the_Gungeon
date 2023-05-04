#pragma once
#include <Engine\CScript.h>
class CTorchScript :
    public CScript
{
private:

public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CTorchScript);

    CTorchScript();
    ~CTorchScript();
};

