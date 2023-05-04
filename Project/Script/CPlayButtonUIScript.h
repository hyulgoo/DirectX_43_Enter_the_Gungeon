#pragma once
#include <Engine\CScript.h>
class CPlayButtonUIScript :
    public CScript
{
private:
    CGameObject* m_pPlayButton;
    bool         m_bMouseOn;
public:
    virtual void begin() override;
    virtual void tick() override;
private:

    CLevel* LoadLevel(const wstring& _LevelPath);
    CGameObject* LoadGameObject(FILE* _File);

    CLONE(CPlayButtonUIScript);

    CPlayButtonUIScript();
    ~CPlayButtonUIScript();
};

