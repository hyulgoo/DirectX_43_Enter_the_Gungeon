#include "pch.h"
#include "CStateScript.h"
#include "CState.h"

CStateScript::CStateScript()
	: CScript((UINT)SCRIPT_TYPE::STATESCRIPT)
	, m_Stat{}
	, m_mapState{}
	, m_pCurState(nullptr)
	, m_bNeedChange(false)
	, m_strChangeState{}
	, m_fDelay(0.f)
{
}

CStateScript::CStateScript(const CStateScript& _Other)
	: CScript((UINT)SCRIPT_TYPE::STATESCRIPT)
	, m_Stat{ _Other .m_Stat}
	, m_mapState{}
	, m_pCurState(nullptr)
	, m_bNeedChange(false)
	, m_strChangeState{}
	, m_fDelay(0.f)
{
}

CStateScript::~CStateScript()
{
	Safe_Del_Map(m_mapState);
}

void CStateScript::begin()
{
}

void CStateScript::tick()
{
	if (m_bNeedChange)
		ChangeState(m_strChangeState);
	
	if (nullptr == m_pCurState)
		return;
	if(0.f < m_fDelay)
		m_fDelay -= DT;


	m_pCurState->tick();
}

void CStateScript::AddState(const wstring& _strKey, CState* _pState)
{
	m_mapState.insert(make_pair(_strKey, _pState));
	_pState->m_OwnerScript = this;
}


void CStateScript::ChangeState(const wstring& _strStateName)
{
	CState* pNextState = FindState(_strStateName);
	assert(pNextState);

	if (nullptr != m_pCurState)
		m_pCurState->Exit();

	m_pCurState = pNextState;

	m_pCurState->Enter();

	m_bNeedChange = false;
}

void CStateScript::SetWeaponType(WEAPON_TYPE _Type)
{
	for (const auto& pair : m_mapState)
	{
		pair.second->m_WeaponType = _Type;
	}
}

void CStateScript::PlayAnim()
{
	m_pCurState->PlayAnim();
}

void CStateScript::BeginOverlap(CCollider2D* _Other)
{
	m_pCurState->BeginOverlap(_Other);
}

void CStateScript::OnOverlap(CCollider2D* _Other)
{
	m_pCurState->OnOverlap(_Other);
}

void CStateScript::EndOverlap(CCollider2D* _Other)
{
	m_pCurState->EndOverlap(_Other);
}

void CStateScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_Stat, sizeof(tStatus), 1, _File);
}

void CStateScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_Stat, sizeof(tStatus), 1, _File);
}
