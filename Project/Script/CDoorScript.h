#pragma once
#include <Engine\CScript.h>

enum class DOOR
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

class CDoorScript :
    public CScript
{
private:
    wstring  m_strAnimName[2];
    int      m_iRoomNum;
    int      m_iType;
    bool     m_bEnter;
    bool     m_bCreate;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetRoomNum(int _iRoomNum) { m_iRoomNum = _iRoomNum; }
    void SetDoorType(int _iDoorType);
    void CloseDoor();
    void OpenDoor();

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CDoorScript);
public:
    CDoorScript();
    CDoorScript(const CDoorScript& _Other);
    ~CDoorScript();

    friend class CWaveMgr;
};

