#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;

class CLevel :
    public CEntity
{
private:
    CLayer*         m_arrLayer[MAX_LAYER];
    LEVEL_STATE     m_State;

public:
    void begin();
    void tick();
    void finaltick();

public:
    CLayer* FindLayerByName(const wstring& _strName);

    CGameObject* FindObjectByName(const wstring& _Name);
    void FindObjectByName(const wstring& _Name, vector<CGameObject*>& _Out);

    void AddGameObject(CGameObject* _Object, int _iLayerIdx, bool _bMove);
    void AddGameObject(CGameObject* _Object, const wstring& _LayerName, bool _Move);

    CLayer* GetLayer(int _iLayerIdx) { assert(!(_iLayerIdx < 0)); return m_arrLayer[_iLayerIdx]; }

    void ChangeState(LEVEL_STATE _State);
    LEVEL_STATE GetState() { return m_State; }

private:
    // finaltick에서 등록되는 오브젝트 초기화
    void clear();


    CLONE(CLevel);
public:
    CLevel();
    ~CLevel();

    friend class CLevelMgr;
};

