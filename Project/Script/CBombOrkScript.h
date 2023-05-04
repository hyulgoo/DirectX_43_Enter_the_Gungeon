#pragma once
#include <Engine\CScript.h>
class CBombOrkScript :
    public CScript
{
private:
    wstring     m_strAnimName;

    virtual void begin() override;
    virtual void tick() override;


    virtual void BeginOverlap(CCollider2D* _Other) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CBombOrkScript);

    CBombOrkScript();
    ~CBombOrkScript();
};

