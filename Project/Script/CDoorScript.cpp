#include "pch.h"
#include "CDoorScript.h"
#include <Engine/CAnim2D.h>
#include "CWaveMgr.h"

CDoorScript::CDoorScript()
	: CScript((UINT)SCRIPT_TYPE::DOORSCRIPT)
	, m_strAnimName{}
    , m_iRoomNum(-1)
    , m_iType(-1)
	, m_bEnter(false)
	, m_bCreate(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_iRoomNum, "RoomNum");
	AddScriptParam(SCRIPT_PARAM::INT, &m_iType, "Type");
}

CDoorScript::CDoorScript(const CDoorScript& _Other)
	: CScript((UINT)SCRIPT_TYPE::DOORSCRIPT)
	, m_strAnimName{}
	, m_iRoomNum(_Other.m_iRoomNum)
	, m_iType(_Other.m_iType)
	, m_bEnter(false)
	, m_bCreate(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, &m_iRoomNum, "RoomNum");
	AddScriptParam(SCRIPT_PARAM::INT, &m_iType, "Type");
}

CDoorScript::~CDoorScript()
{
	CWaveMgr::GetInst()->DeleteDoor(GetOwner());
}
void CDoorScript::begin()
{
	if (L"Door_Up" == GetOwner()->GetName())
		m_iType = (int)DOOR::UP;
	else if (L"Door_Down" == GetOwner()->GetName())
		m_iType = (int)DOOR::DOWN;
	else if (L"Door_Left" == GetOwner()->GetName())
		m_iType = (int)DOOR::LEFT;
	else if (L"Door_Right" == GetOwner()->GetName())
		m_iType = (int)DOOR::RIGHT;
	SetDoorType(m_iType);
	MeshRender()->GetDynamicMaterial();
	CWaveMgr::GetInst()->RegisterDoor(m_iRoomNum, GetOwner());
	Animator2D()->LoadAnimFromFile(GetOwner()->GetName());
	Animator2D()->Play(m_strAnimName[1], false);
	if((int)DOOR::UP == m_iType || (int)DOOR::DOWN == m_iType)
		Animator2D()->GetCurAnim()->Stop(2);	
	else if((int)DOOR::LEFT == m_iType || (int)DOOR::RIGHT == m_iType)
		Animator2D()->GetCurAnim()->Stop(2);
}

void CDoorScript::tick()
{
}

void CDoorScript::SetDoorType(int _iDoorType)
{
	// 4 이상의 숫자가 들어오면 animname이 설정되지 않으므로 assert
	assert(_iDoorType < 4);

	m_iType = _iDoorType;

	switch ((DOOR)_iDoorType)
	{
	case DOOR::UP:
	{
		m_strAnimName[0] = L"Door_Up_Open";
		m_strAnimName[1] = L"Door_Up_Close";
	}
		break;
	case DOOR::LEFT:
	{
		m_strAnimName[0] = L"Door_Left_Open";
		m_strAnimName[1] = L"Door_Left_Close";
	}
		break;
	case DOOR::RIGHT:
	{
		m_strAnimName[0] = L"Door_Right_Open";
		m_strAnimName[1] = L"Door_Right_Close";
	}
		break;
	case DOOR::DOWN:
	{
		m_strAnimName[0] = L"Door_Down_Open";
		m_strAnimName[1] = L"Door_Down_Close";
	}
		break;
	}
}

void CDoorScript::CloseDoor()
{
	if(!m_bCreate)
	{
		GetOwner()->AddComponent(new CCollider2D);
		Collider2D()->SetAbsolute(true);
		m_bCreate = true;
	}
	if ((int)DOOR::DOWN == m_iType || (int)DOOR::UP == m_iType)
		Collider2D()->SetOffsetScale(Vec2(128.f, 32.f));
	else if(((int)DOOR::LEFT == m_iType || (int)DOOR::RIGHT == m_iType))
		Collider2D()->SetOffsetScale(Vec2(32.f, 128.f));
	Animator2D()->Play(m_strAnimName[1], false);
	CResMgr::GetInst()->FindRes<CSound>(L"DoorClosed")->Play(1, 0.25f); 
}

void CDoorScript::OpenDoor()
{
	Animator2D()->Play(m_strAnimName[0], false);
	if(m_bEnter)
	{
		GetOwner()->DeleteComponent((UINT)COMPONENT_TYPE::COLLIDER2D);
	}
	CResMgr::GetInst()->FindRes<CSound>(L"Door_Open")->Play(1, 0.25f, true);
	m_bEnter = true;
}

void CDoorScript::SaveToLevelFile(FILE* _File)
{	
	fwrite(&m_iRoomNum, sizeof(int), 1, _File);
	fwrite(&m_iType, sizeof(int), 1, _File);
}

void CDoorScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_iRoomNum, sizeof(int), 1, _File);
	fread(&m_iType, sizeof(int), 1, _File);
	SetDoorType(m_iType);
}

