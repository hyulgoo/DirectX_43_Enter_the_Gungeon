#pragma once
#include "UI.h"

class MenuUI :
    public UI
{
private:
    CGameObject* m_pCurColliderObj;
    bool         m_bWall;
    Vec2         m_WallLtp;
    Vec2         m_WallBtm;
public:
    virtual void finaltick() override;
    virtual int render_update() override;

private:
    void CreateEmptyObject();
    void CreateWall();
    void AddComponent(COMPONENT_TYPE _type);
    void AddScript(const wstring& _strScriptName);

public:
    MenuUI();
    ~MenuUI();
};

