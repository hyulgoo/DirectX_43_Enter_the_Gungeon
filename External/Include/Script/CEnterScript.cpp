#include "pch.h"
#include "CEnterScript.h"
#include "CWaveMgr.h"

CEnterScript::CEnterScript()
	: CScript((UINT)SCRIPT_TYPE::ENTERSCRIPT)
	, m_iRoomNum(0)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_iRoomNum, "RoomNum");
}

CEnterScript::CEnterScript(const CEnterScript& _Other)
	: CScript((UINT)SCRIPT_TYPE::ENTERSCRIPT)
	, m_iRoomNum(_Other.m_iRoomNum)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_iRoomNum, "RoomNum");
}

CEnterScript::~CEnterScript()
{
}

void CEnterScript::begin()
{
}

void CEnterScript::tick()
{
}

void CEnterScript::BeginOverlap(CCollider2D* _Other)
{
	CWaveMgr::GetInst()->OpenDoor(m_iRoomNum);
	SetLifeSpan(0.f);
}

void CEnterScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_iRoomNum, sizeof(int), 1, _File);
}

void CEnterScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_iRoomNum, sizeof(int), 1, _File);
}
