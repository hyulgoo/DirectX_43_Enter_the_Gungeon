#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :
    public CEntity
{
private:
    vector<CGameObject*>    m_vecParentObj; // 부모 오브젝트
    vector<CGameObject*>    m_vecObject;    // 모든 오브젝트
    int                     m_iLayerIdx;    // 헤이어 번호

public:
    void begin();
    void tick();
    void finaltick();

public:
    int GetLayerIdx() { return m_iLayerIdx; }
    const vector<CGameObject*>& GetParentObject() { return m_vecParentObj; }
    const vector<CGameObject*>& GetObjects() { return m_vecObject; }

    // 레이어 소속을 바꾸는 것은 다른 곳에서 함부로 호출되면 안되기 때문에 private으로 하고
    // Level, GameObject(자신을 바꿀 수 있게), EventMgr에서만 할 수 있도록 한다.
private:
    void AddGameObject(CGameObject* _Object, bool _bMove);
    void RegisterObject(CGameObject* _Object) { m_vecObject.push_back(_Object); }
    void RemoveFromParentList(CGameObject* _Obj);
    void AddParentList(CGameObject* _Obj);

    CLONE(CLayer);
public:
    CLayer();
    ~CLayer();

    friend class CLevel;
    friend class CGameObject;
    friend class CEventMgr;
};

