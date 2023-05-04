#pragma once
#include <Engine\CScript.h>
class CLight2DScript :
    public CScript
{
public:
    virtual void tick() override;

    CLONE(CLight2DScript);
public:
    CLight2DScript();
    ~CLight2DScript();
};

