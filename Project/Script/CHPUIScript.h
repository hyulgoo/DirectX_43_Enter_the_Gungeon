#pragma once
#include <Engine\CScript.h>

enum class HPSTATE
{
    FULL,
    HALF,
    EMPTY,
};
class CHPUIScript :
    public CScript
{
private:
    vector<CGameObject*> m_vecHP;
    vector<CGameObject*> m_vecBlank;
public:
    virtual void begin() override;
    virtual void tick() override;

    void SetHP(int _iHP);
    void SetBlank(int _iBlankCount);

    CLONE(CHPUIScript);

    CHPUIScript();
    CHPUIScript(const CHPUIScript& _Other);
    ~CHPUIScript();
};

