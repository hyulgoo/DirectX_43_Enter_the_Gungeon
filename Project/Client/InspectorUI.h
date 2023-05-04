#pragma once
#include "UI.h"

class CGameObject;
class ComponentUI;
class ResUI;
class ScriptUI;

#include <Engine\ptr.h>
#include <Engine\CRes.h>


class InspectorUI :
    public UI
{
private:
    CGameObject*        m_pTargetObj;
    ComponentUI*        m_arrComUI[(UINT)COMPONENT_TYPE::END];
    vector<ScriptUI*>   m_vecScriptUI;

    Ptr<CRes>           m_pTargetRes;
    ResUI*              m_arrResUI[(UINT)RES_TYPE::END];

    char                m_NewName[50];
    int                 m_iLayerNum;

public:
    virtual void init() override;
    virtual void tick() override;
    virtual int render_update() override;

public:
    void SetTargetObject(CGameObject* _Target);
    void SetTargetResource(Ptr<CRes> _Res);
    int GetSpawnLayerNum() { return m_iLayerNum; }
    void SetSpawnLayerNum(int _iLayerNum) 
    {
        if (0 <= _iLayerNum && _iLayerNum <= MAX_LAYER)
        {
            m_iLayerNum = _iLayerNum;
        }
    }

private:
    void ClearTargetObject();
    void ClearTargetResource();
    void SetTargetName();
    void AddTargetToPrefab();
    void SpawnPrefab();

public:
    InspectorUI();
    ~InspectorUI();
};

