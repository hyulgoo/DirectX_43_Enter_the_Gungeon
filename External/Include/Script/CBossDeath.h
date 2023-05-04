#pragma once
#include "CState.h"
class CBossDeath :
    public CState
{
private:

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    void SetAnimName();
    virtual void PlayAnim() override;
    CLONE(CBossDeath);

    CLevel* LoadLevel(const wstring& _LevelPath);
    CGameObject* LoadGameObject(FILE* _File);

public:
    CBossDeath();
    ~CBossDeath();
};

