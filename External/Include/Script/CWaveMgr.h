#pragma once
#include <Engine\CSingleton.h>

class CWaveScript;

struct DoorInfo
{
    int          RoomNum;
    CGameObject* Obj;
};

class CWaveMgr :
    public CSingleton<CWaveMgr>
{
    SINGLE(CWaveMgr);
private:
    int                     m_iCurDoorNum;
    vector<DoorInfo>        m_vecDoor;
    map<int, CWaveScript*>  m_mapWave;

public:
    void RegisterDoor(int _iRoomNum, CGameObject* _pDoor);
    void RegisterWave(int _iRoomNum, CWaveScript* _pWave);
    void DeleteDoor(CGameObject* _pDoor);

    void ActiveRoom(int _iRoomNum);

    void OpenDoor(int _iRoomNum);
    void CloseDoor(int _iRoomNum);

    void MinusMstCount();

    bool IsExist(CGameObject* _pObj);
};

