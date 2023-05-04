#pragma once
#include <Engine/CSingleton.h>

class CGameObject;

struct DoorInfo
{
    int RoomNum;
    CGameObject* Obj;
};

class CWaveMgr :
    public CSingleton<CWaveMgr>
{
    SINGLE(CWaveMgr);
private:
    vector<DoorInfo> m_vecWave;
    vector<DoorInfo> m_vecDoor;

public:
    void RegisterWaveObj(int _iRoomNum, CGameObject* _pWave);
    void RegisterDoorObj(int _iRoomNum, CGameObject* _pDoor);

    void OpenDoor(int _iRoomNum);
    void CloseDoor(int _iRoomNum);
};