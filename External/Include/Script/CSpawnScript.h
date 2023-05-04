#pragma once
#include <Engine\CScript.h>
class CSpawnScript :
    public CScript
{
private:
    wstring     m_strSpawnMstName;
    int         m_iSpawnMstNum;
    float       m_fDelay;
    bool        m_bSpawn;

public:
    virtual void begin() override;
    virtual void tick() override;
    void SetSpawnMst(int _strSpawnNum) { m_iSpawnMstNum = _strSpawnNum; }

    CLONE(CSpawnScript);
public:
    CSpawnScript();
    ~CSpawnScript();
};

