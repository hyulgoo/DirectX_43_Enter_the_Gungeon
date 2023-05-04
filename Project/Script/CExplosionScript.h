#pragma once
#include <Engine\CScript.h>
class CExplosionScript :
    public CScript
{
private:
    wstring     m_strAnimName;
    bool        m_bBomb;

    virtual void begin() override;
    virtual void tick() override;
    void SetAnimName(wstring _strName) { m_strAnimName = _strName; }


    virtual void BeginOverlap(CCollider2D* _Other) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CExplosionScript);

    CExplosionScript();
    ~CExplosionScript();
};

