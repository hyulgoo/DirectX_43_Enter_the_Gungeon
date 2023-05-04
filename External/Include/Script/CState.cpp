#include "pch.h"
#include "CState.h"

CState::CState()
 : m_OwnerScript(nullptr)
 , m_WeaponType()
 , m_MonsterType()
 , m_Dir()
 , m_vDirtoPlayer{}
 , m_fDistancetoPlayer(0.f)
{
}

CState::~CState()
{
}

void CState::ChangeState(wstring _strStateName)
{
	m_OwnerScript->SetChange(_strStateName);
}

void CState::SetDir(DIR _dir)
{
	m_Dir = _dir;
	PlayAnim();
}
