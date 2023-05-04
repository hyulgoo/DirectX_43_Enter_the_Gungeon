#pragma once
#include <Engine\CScript.h>
class CBossHPScript :
    public CScript
{
private:
    CGameObject* m_pBossHPBar;
    CGameObject* m_pBossHPFrame;
public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void SetBossHP(int _iHP);
    void BossAppear();

    CLONE(CBossHPScript);

    CBossHPScript();
    ~CBossHPScript();

    friend class CBossScript;
    friend class CUIMgr;
};

