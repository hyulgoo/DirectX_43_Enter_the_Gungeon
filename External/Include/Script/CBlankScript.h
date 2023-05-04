#pragma once
#include <Engine\CScript.h>
class CBlankScript :
    public CScript
{
private:
    float   m_fDelay;
    float   m_WaveTime;
    float   m_bDeleteCollider;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;

    CLONE(CBlankScript);

public:
    CBlankScript();
    ~CBlankScript();
};

