#pragma once
#include <Engine\CEntity.h>
#include "CStateScript.h"

class CStateScript;

class CState :
    public CEntity
{
    CStateScript*   m_OwnerScript;
    WEAPON_TYPE     m_WeaponType;
    MONSTER_TYPE    m_MonsterType;
    DIR             m_Dir;
    Vec2            m_vDirtoPlayer;
    float           m_fDistancetoPlayer;


public:
    virtual void tick() = 0;
    virtual void Enter() = 0;
    virtual void Exit() = 0;

public:
    void ChangeState(wstring _strStateName);
    void SetDirtoPlayer(Vec2 _vMtP) { m_vDirtoPlayer = _vMtP; }
    Vec2 GetDirtoPlayer() { return m_vDirtoPlayer; }

    void SetDistancetoPlayer(float _vMtP) { m_fDistancetoPlayer = _vMtP; }
    float GetDistancetoPlayer() { return m_fDistancetoPlayer; }

    CStateScript* GetOwnerScript() { return m_OwnerScript; }
    CGameObject* GetOwner() { return m_OwnerScript->GetOwner(); }

    WEAPON_TYPE GetType() { return m_WeaponType; }

    virtual void PlayAnim() {};
    virtual void SetAnimName(wstring _strUp, wstring _strDown, wstring _strLeftUp, wstring _strLeftDown, wstring _strRightUp, wstring _strRightDown) {};

    void SetDir(DIR _dir);
    DIR GetDir() { return m_Dir; }

    virtual void BeginOverlap(CCollider2D* _Other) {};
    virtual void OnOverlap(CCollider2D* _Other) {};
    virtual void EndOverlap(CCollider2D* _Other) {};

public:
    CState* Clone() = 0;

    CState();
    ~CState();
    friend class CStateScript;
};

