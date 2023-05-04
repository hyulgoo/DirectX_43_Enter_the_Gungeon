#pragma once
#include <Engine\CScript.h>

class CState;

class CStateScript :
    public CScript
{
private:
    map<wstring, CState*>   m_mapState;
    CState*                 m_pCurState;
    tStatus                 m_Stat;
    bool                    m_bNeedChange;
    wstring                 m_strChangeState;
    float                   m_fDelay;

public:
    virtual void begin() override;
    virtual void tick() override;

    void AddState(const wstring& _strKey, CState* _pState);
    CState* GetCurState() { return m_pCurState; }
    CState* FindState(const wstring& _strKey)
    {
        map<wstring, CState*>::iterator iter = m_mapState.find(_strKey);
        if (iter != m_mapState.end())
            return iter->second;
        return nullptr;
    }
    void SetChange(const wstring& _strStateName)
    {
        m_bNeedChange = true;
        m_strChangeState = _strStateName;
    }
    void ChangeState(const wstring& _strStateName);

    tStatus GetStat() { return m_Stat; }
    void SetStat(tStatus _status) { m_Stat = _status; }

    void SetDelay(float _fDelay) { m_fDelay = _fDelay; }
    bool IsDelayEnd() { bool bEnd = m_fDelay < 0.f ? true : false; return bEnd; }

    void SetWeaponType(WEAPON_TYPE _Type);
    void PlayAnim();

    virtual void BeginOverlap(CCollider2D* _Other);
    virtual void OnOverlap(CCollider2D* _Other);
    virtual void EndOverlap(CCollider2D* _Other);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CStateScript);
public:
    CStateScript();
    CStateScript(const CStateScript& _Other);
    ~CStateScript();

};

