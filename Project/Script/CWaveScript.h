#pragma once
#include <Engine\CScript.h>

class CWaveScript :
    public CScript
{
private:
    int                 m_iRoomNum;
    int                 m_iRemainMst;
    int                 m_iCurWaveNum;
    int                 m_iMaxWaveNum;
    vector<WaveInfo>    m_vecWave[3];
    bool                m_bActive;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void SpawnMst();

public:
    void SetWaveCount(int _iWaveCount) { m_iMaxWaveNum = _iWaveCount; }
    void SetRoomNum(int _iRoomNum) { m_iRoomNum = _iRoomNum; }
    void SetWaveInfo(int _iWaveNum, vector<WaveInfo> _mapInfo);
    void MinusMstCount();
    vector<WaveInfo> GetWaveInfo(int _iWaveNum) { return m_vecWave[_iWaveNum]; }
    void AddWaveMst(int _iWavwNum, int _iPrefNum, Vec2 _vSpawnPos);
    virtual void BeginOverlap(CCollider2D* _Other) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CWaveScript);
public:
    CWaveScript();
    CWaveScript(const CWaveScript& _Other);
    ~CWaveScript();
    friend class CWaveMgr;
};

