#include "pch.h"
#include "CWaveScript.h"
#include "CSpawnScript.h"
#include "CWaveMgr.h"

CWaveScript::CWaveScript()
	: CScript((UINT)SCRIPT_TYPE::WAVESCRIPT)
	, m_iRoomNum(-1)
	, m_iRemainMst(-1)
	, m_iCurWaveNum(0)
	, m_iMaxWaveNum(0)
	, m_vecWave{}
	, m_bActive(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_iRoomNum, "RoomNum");
}

CWaveScript::CWaveScript(const CWaveScript& _Other)
	: CScript((UINT)SCRIPT_TYPE::WAVESCRIPT)
	, m_iRoomNum(_Other.m_iRoomNum)
	, m_iRemainMst(-1)
	, m_iCurWaveNum(0)
	, m_iMaxWaveNum(_Other.m_iMaxWaveNum)
	, m_vecWave()
	, m_bActive(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_iRoomNum, "RoomNum");
}

CWaveScript::~CWaveScript()
{
	for (size_t i = 0; i < m_vecWave->size(); ++i)
	{
		m_vecWave[i].clear();
	}
}

void CWaveScript::begin()
{
	CWaveMgr::GetInst()->RegisterWave(m_iRoomNum, this);
}

void CWaveScript::tick()
{
}

void CWaveScript::SpawnMst()
{
	for (size_t i = 0; i < m_vecWave[m_iCurWaveNum].size(); ++i)
	{
		CGameObject* pMst = CResMgr::GetInst()->FindRes<CPrefab>(L"Spawner")->Instantiate();
		int iPrefabNum = m_vecWave[m_iCurWaveNum][i].PrefabNum;
		Vec2 Pos = m_vecWave[m_iCurWaveNum][i].SpawnPos;
		Vec3 vPos = Vec3(Pos.x, Pos.y, 300.f);
		pMst->GetScript<CSpawnScript>()->SetSpawnMst(iPrefabNum);
		SpawnGameObject(pMst, vPos, (int)LAYER::DEFAULT);
	}
	m_iRemainMst = (int)m_vecWave[m_iCurWaveNum].size();
	++m_iCurWaveNum;
}
void CWaveScript::SetWaveInfo(int _iWaveNum, vector<WaveInfo> _mapInfo)
{
	m_vecWave[_iWaveNum] = _mapInfo;
}

void CWaveScript::MinusMstCount()
{
	--m_iRemainMst;
	if (0 == m_iRemainMst)
	{
		if (m_iCurWaveNum < m_iMaxWaveNum)
			SpawnMst();
		else
			CWaveMgr::GetInst()->OpenDoor(m_iRoomNum);
	}
}

void CWaveScript::AddWaveMst(int _iWavwNum, int _iPrefNum, Vec2 _vSpawnPos)
{
	WaveInfo Info;
	Info.PrefabNum = _iPrefNum;
	Info.SpawnPos = _vSpawnPos;
	m_vecWave[_iWavwNum].push_back(Info);
}

void CWaveScript::BeginOverlap(CCollider2D* _Other)
{
	if(!m_bActive)
	{
		CWaveMgr::GetInst()->ActiveRoom(m_iRoomNum);
		CWaveMgr::GetInst()->CloseDoor(m_iRoomNum);
		m_bActive = true;
	}
}

void CWaveScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_iRoomNum, sizeof(int), 1, _File);
	fwrite(&m_iMaxWaveNum, sizeof(int), 1, _File);
	for (size_t i = 0; i < 3; ++i)
	{
		size_t size = m_vecWave[i].size();
		fwrite(&size, sizeof(size_t), 1, _File);

		for (size_t j = 0; j < size; ++j)
		{
			fwrite(&m_vecWave[i][j].PrefabNum, sizeof(int), 1, _File);
			fwrite(&m_vecWave[i][j].SpawnPos, sizeof(Vec2), 1, _File);
		}
	}
}

void CWaveScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_iRoomNum, sizeof(int), 1, _File);
	fread(&m_iMaxWaveNum, sizeof(int), 1, _File);
	for (size_t i = 0; i < 3; ++i)
	{
		size_t  size = 0;
		fread(&size, sizeof(size_t), 1, _File);
		m_vecWave[i].resize(size);
		for (size_t j = 0; j < size; ++j)
		{
			fread(&m_vecWave[i][j].PrefabNum, sizeof(int), 1, _File);
			fread(&m_vecWave[i][j].SpawnPos, sizeof(Vec2), 1, _File);
		}
	}
}
