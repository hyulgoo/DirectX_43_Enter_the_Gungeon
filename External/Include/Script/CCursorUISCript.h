#pragma once
#include <Engine\CScript.h>
class CCursorUISCript :
    public CScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

    CLONE(CCursorUISCript);

    CCursorUISCript();
    ~CCursorUISCript();
};

