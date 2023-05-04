#include "pch.h"
#include "CWaveMgr.h"
#include <Engine/CGameObject.h>
#include "CDoorScript"


CWaveMgr::CWaveMgr()
{

}
CWaveMgr::~CWaveMgr()
{

}

void CWaveMgr::RegisterWaveObj(int _iRoomNum, CGameObject* _pWave)
{
	DoorInfo Room;
	Room.RoomNum = _iRoomNum;
	Room.Obj = _pWave;
	m_vecWave.push_back(Room);
}

void CWaveMgr::RegisterDoorObj(int _iRoomNum, CGameObject* _pDoor)
{
	DoorInfo Room;
	Room.RoomNum = _iRoomNum;
	Room.Obj = _pDoor;
	m_vecDoor.push_back(Room);
}

void CWaveMgr::OpenDoor(int _iRoomNum)
{
	for (size_t i = 0; i < m_vecDoor.size(); ++i)
	{
		if (_iRoomNum == m_vecDoor[i].RoomNum)
			m_vecDoor[i].Obj->GetScript<CDoorScript>()->OpenDoor();
	}
}

void CWaveMgr::CloseDoor(int _iRoomNum)
{
	for (size_t i = 0; i < m_vecDoor.size(); ++i)
	{
		if (_iRoomNum == m_vecDoor[i].RoomNum)
			m_vecDoor[i].Obj->GetScript<CDoorScript>()->OpenDoor();
	}
}
