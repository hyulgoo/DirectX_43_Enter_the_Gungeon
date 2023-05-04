#pragma once
#include <Engine\CScript.h>
class CFragileScript :
    public CScript
{
private:
    wstring     m_strAnimName;
    bool        m_bHit;
    bool        m_bBomb;

public:
    virtual void begin() override;
    virtual void tick() override;
    void SetAnimName(wstring _strName) { m_strAnimName = _strName; }


    virtual void BeginOverlap(CCollider2D* _Other) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CFragileScript);

    CFragileScript();
    CFragileScript(const CFragileScript& _Other);
    ~CFragileScript();
};

