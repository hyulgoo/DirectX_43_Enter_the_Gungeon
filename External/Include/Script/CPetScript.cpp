#include "pch.h"
#include "CPetScript.h"
#include "CPetIdle.h"
#include "CPetTrace.h"

#include "CStateScript.h"

CPetScript::CPetScript()
	: CScript((UINT)SCRIPT_TYPE::PETSCRIPT)
	, m_pState(nullptr)
	, m_Dir{}
	, m_pPlayer(nullptr)
	, m_fLengthtoPly(0.f)
	, m_fAngle(0.f)
	, m_vDirToPly{}
	, m_vPlayerPos{}
{
	m_Dir.CurDir = DIR::DOWN;
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fAngle, "Angle");
}

CPetScript::~CPetScript()
{
}

void CPetScript::begin()
{
	MeshRender()->GetDynamicMaterial();
	SetState();
	m_pPlayer = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player");
}

void CPetScript::tick()
{
	m_Dir.PrevDir = m_Dir.CurDir;

	CheckDir();
}

void CPetScript::CheckDir()
{
	Vec3 PetPos = Transform()->GetWorldPos();
	m_vPlayerPos = m_pPlayer->Transform()->GetWorldPos();

	m_fAngle = GetDirfloat(PetPos, m_vPlayerPos, false);
	m_fLengthtoPly = Vec2(m_vPlayerPos.x - PetPos.x, m_vPlayerPos.y - PetPos.y).Length();
	m_fAngle *= (180 / XM_PI);

	float Rot = (m_fAngle <= 180.f ? 0.f : XM_PI);
	Transform()->SetRelativeRot(Vec3(0.f, Rot, 0.f));

	if (20.f >= m_fAngle || 340.f < m_fAngle)
	{
		if (DIR::DOWN != m_Dir.CurDir)
		{
			m_Dir.CurDir = DIR::DOWN;
			m_pState->GetCurState()->SetDir(DIR::DOWN);
		}
	}

	if (20.f < m_fAngle && m_fAngle <= 90.f)
	{
		if (DIR::RIGHT_DOWN != m_Dir.CurDir)
		{
			m_Dir.CurDir = DIR::RIGHT_DOWN;
			m_pState->GetCurState()->SetDir(DIR::RIGHT_DOWN);
		}
	}

	if (90.f < m_fAngle && m_fAngle <= 160.f)
	{
		if (DIR::RIGHT_UP != m_Dir.CurDir)
		{
			m_Dir.CurDir = DIR::RIGHT_UP;
			m_pState->GetCurState()->SetDir(DIR::RIGHT_UP);
		}
	}

	if (160.f < m_fAngle && m_fAngle <= 200.f)
	{
		if (DIR::UP != m_Dir.CurDir)
		{
			m_Dir.CurDir = DIR::UP;
			m_pState->GetCurState()->SetDir(DIR::UP);
		}
	}

	if (200.f < m_fAngle && m_fAngle <= 270.f)
	{
		if (DIR::LEFT_UP != m_Dir.CurDir)
		{
			m_Dir.CurDir = DIR::LEFT_UP;
			m_pState->GetCurState()->SetDir(DIR::LEFT_UP);
		}
	}

	if (270.f < m_fAngle && m_fAngle <= 340.f)
	{
		if (DIR::LEFT_DOWN != m_Dir.CurDir)
		{
			m_Dir.CurDir = DIR::LEFT_DOWN;
			m_pState->GetCurState()->SetDir(DIR::LEFT_DOWN);
		}
	}
}

void CPetScript::SetState()
{
	if (m_pState != nullptr)
		return;

	Animator2D()->LoadAnimFromFile(L"Pet");
	m_pState = GetOwner()->GetScript<CStateScript>();
	m_pState->AddState(L"Idle", new CPetIdle);
	m_pState->AddState(L"Trace", new CPetTrace);
	m_pState->FindState(L"Idle")->SetAnimName(L"PET_IDLE_RIGHT", L"PET_IDLE_RIGHT", L"PET_IDLE_RIGHT", L"PET_IDLE_RIGHT", L"PET_IDLE_RIGHT", L"PET_IDLE_RIGHT");
	m_pState->FindState(L"Trace")->SetAnimName(L"PET_WALK_UP", L"PET_WALK_DOWN", L"PET_WALK_RIGHT_UP", L"PET_WALK_RIGHT_DOWN", L"PET_WALK_RIGHT_UP", L"PET_WALK_RIGHT_DOWN");
	m_pState->ChangeState(L"Idle");
	m_pState->PlayAnim();

	tStatus PetStat;
	PetStat.Speed = 200;
	m_pState->SetStat(PetStat);
}
