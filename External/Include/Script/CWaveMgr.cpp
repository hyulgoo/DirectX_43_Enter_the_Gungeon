#include "pch.h"
#include "CWaveMgr.h"
#include <Engine/CGameObject.h>
#include "CDoorScript.h"
#include "CWaveScript.h"

CWaveMgr::CWaveMgr()
	: m_iCurDoorNum(-1)
	, m_vecDoor{}
	, m_mapWave{}
{
}

CWaveMgr::~CWaveMgr()
{

}

void CWaveMgr::RegisterDoor(int _iRoomNum, CGameObject* _pDoor)
{
	assert(_iRoomNum != -1);
	DoorInfo Info;
	Info.RoomNum = _iRoomNum;
	Info.Obj = _pDoor;
	m_vecDoor.push_back(Info);
}

void CWaveMgr::RegisterWave(int _iRoomNum, CWaveScript* _pWave)
{	
	assert(_iRoomNum != -1);
	if (_iRoomNum == 1)
		m_mapWave.clear();
	m_mapWave.insert(make_pair(_iRoomNum, _pWave));
}

void CWaveMgr::DeleteDoor(CGameObject* _pDoor)
{
	vector<DoorInfo>::iterator iter = m_vecDoor.begin();
	for (; iter != m_vecDoor.end(); ++iter)
	{
		if (iter->Obj == _pDoor)
		{
			m_vecDoor.erase(iter);
			return;
		}
	}
}

void CWaveMgr::ActiveRoom(int _iRoomNum)
{ 
	m_iCurDoorNum = _iRoomNum;
	m_mapWave.find(m_iCurDoorNum)->second->SpawnMst();
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
			m_vecDoor[i].Obj->GetScript<CDoorScript>()->CloseDoor();
	}
}

void CWaveMgr::MinusMstCount()
{
	CWaveScript* Wave = m_mapWave.find(m_iCurDoorNum)->second;
	Wave->MinusMstCount();
}

bool CWaveMgr::IsExist(CGameObject* _pObj)
{
	for (size_t i = 0; i < m_vecDoor.size(); ++i)
	{
		if (m_vecDoor[i].Obj == _pObj)
			return true;
	}
	return false;
}
