#pragma once
#include "ResUI.h"

class CPrefab;

class PrefabUI :
    public ResUI
{
private:
    int     m_iLayerNum;

public:
    virtual int render_update() override;

private:
    int SavePrefab(const wstring& _PrefabPath);
    //CPrefab* LoadPrefab(const wstring& _PrefabPath);
    void ShowOwnComponent(CGameObject* _Obj);

public:
    PrefabUI();
    ~PrefabUI();
};
