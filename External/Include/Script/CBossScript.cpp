#include "pch.h"
#include "CBossScript.h"
#include "CStateScript.h"
#include "CBossIdle.h"
#include "CBossTrace.h"
#include "CBossJump.h"
#include "CBossDeath.h"
#include "CBossCQC.h"
#include "CBossCharge.h"
#include "CBossMissile.h"
#include "CMissileScript.h"
#include "CShadowScript.h"
#include "CUIMgr.h"

CBossScript::CBossScript()
	: CScript((UINT)SCRIPT_TYPE::BOSSSCRIPT)
	, m_pState(nullptr)
	, m_CurDir()
    , m_PrevDir()
    , m_pPlayer(nullptr)
    , m_fLengthtoPly(0.f)
    , m_fAngle(0.f)
	, m_vDirToPly{}
	, m_vPlayerPos{}
	, m_bCollidable(true)
	, m_bRotable(true)
{
}

CBossScript::CBossScript(const CBossScript& _Other)
	: CScript((UINT)SCRIPT_TYPE::BOSSSCRIPT)
	, m_pState(nullptr)
	, m_CurDir()
	, m_PrevDir()
	, m_pPlayer(nullptr)
	, m_fLengthtoPly(0.f)
	, m_fAngle(0.f)
	, m_vDirToPly{}
	, m_vPlayerPos{}
	, m_bCollidable(true)
{
}

CBossScript::~CBossScript()
{
}

void CBossScript::begin()
{
	MeshRender()->GetDynamicMaterial();
	if(nullptr == m_pState)
	{
		m_pState = GetOwner()->GetScript<CStateScript>();
		m_pState->AddState(L"Idle", new CBossIdle);
		m_pState->AddState(L"Trace", new CBossTrace);
		m_pState->AddState(L"Death", new CBossDeath);
		m_pState->AddState(L"ATK_Jump", new CBossJump);
		m_pState->AddState(L"ATK_Charge", new CBossCharge);
		m_pState->AddState(L"ATK_CQC", new CBossCQC);
		m_pState->AddState(L"ATK_Missile", new CBossMissile);
		m_pState->ChangeState(L"Idle");
		m_pState->PlayAnim();

		tStatus Stat;
		Stat.HP = 300;
		m_pState->SetStat(Stat);
	}
	if (nullptr == m_pPlayer)
	{
		m_pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	}
	
}

void CBossScript::tick()
{
	if (m_bHitEffect)
	{
		if (m_fHitEffectDelay < 0.f)
		{
			MeshRender()->SetMaterial(MeshRender()->GetSharedMaterial());
			m_bHitEffect = false;
			return;
		}
		m_fHitEffectDelay -= DT;
	}

	if (m_pState->GetStat().HP <= 0 
		&& m_pState->GetCurState() != m_pState->FindState(L"Death"))
		m_pState->ChangeState(L"Death");
	CheckDir();
}

void CBossScript::CheckDir()
{
	// 플레이어와의 거리, 방향, 각도를 계산함.
	Vec3 MstPos = Transform()->GetWorldPos();
	m_vPlayerPos = m_pPlayer->Transform()->GetWorldPos();

	m_fAngle = GetDirfloat(MstPos, m_vPlayerPos, true);
	m_fLengthtoPly = Vec2(m_vPlayerPos.x - MstPos.x, m_vPlayerPos.y - MstPos.y).Length();
	m_vDirToPly = Vec2(m_vPlayerPos.x - MstPos.x, m_vPlayerPos.y - MstPos.y);
	m_vDirToPly.Normalize();

	if (!m_bRotable)
		return;
	float Rot = 0.f;

		Rot = (m_fAngle <= 180.f ? XM_PI : 0.f);


	Transform()->SetRelativeRot(Vec3(0.f, Rot, 0.f));

	if (23.5f >= m_fAngle || 336.5f < m_fAngle)
	{
		if (DIR::DOWN != m_CurDir)
		{
			m_CurDir = DIR::DOWN;
			m_pState->GetCurState()->SetDir(DIR::DOWN);
		}
	}

	else if (22.5f < m_fAngle && m_fAngle <= 67.5f)
	{
		if (DIR::RIGHT_DOWN != m_CurDir)
		{
			m_CurDir = DIR::RIGHT_DOWN;
			m_pState->GetCurState()->SetDir(DIR::RIGHT_DOWN);
		}
	}

	else if (67.5f < m_fAngle && m_fAngle <= 112.5f)
	{
		if (DIR::RIGHT != m_CurDir)
		{
			m_CurDir = DIR::RIGHT;
			m_pState->GetCurState()->SetDir(DIR::RIGHT);
		}
	}

	else if (112.5f < m_fAngle && m_fAngle <= 157.5f)
	{
		if (DIR::RIGHT_UP != m_CurDir)
		{
			m_CurDir = DIR::RIGHT_UP;
			m_pState->GetCurState()->SetDir(DIR::RIGHT_UP);
		}
	}
	else if (157.5f < m_fAngle && m_fAngle <= 202.5f)
	{
		if (DIR::UP != m_CurDir)
		{
			m_CurDir = DIR::UP;
			m_pState->GetCurState()->SetDir(DIR::UP);
		}
	}

	else if (202.5f < m_fAngle && m_fAngle <= 247.5f)
	{
		if (DIR::LEFT_UP != m_CurDir)
		{
			m_CurDir = DIR::LEFT_UP;
			m_pState->GetCurState()->SetDir(DIR::LEFT_UP);
		}
	}

	else if (247.5f < m_fAngle && m_fAngle <= 292.5f)
	{
		if (DIR::LEFT != m_CurDir)
		{
			m_CurDir = DIR::LEFT;
			m_pState->GetCurState()->SetDir(DIR::LEFT);
		}
	}

	else if (292.5f < m_fAngle && m_fAngle <= 336.5f)
	{
		if (DIR::LEFT_DOWN != m_CurDir)
		{
			m_CurDir = DIR::LEFT_DOWN;
			m_pState->GetCurState()->SetDir(DIR::LEFT_DOWN);
		}
	}
}

void CBossScript::SetState()
{
}

void CBossScript::SetHitEffect()
{
	int a = 1;
	MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_2, &a);
	m_bHitEffect = true;
	m_fHitEffectDelay = 1.f;
}

void CBossScript::BeginOverlap(CCollider2D* _Other)
{
	if(IsCollidable())
	{
		if ((int)LAYER::PLAYERPROJECTILE == _Other->GetOwner()->GetLayerIndex())
		{
			int iDamage = _Other->GetOwner()->GetScript<CMissileScript>()->GetDamage();
			tStatus tStat = m_pState->GetStat();
			tStat.HP -= iDamage;
			m_pState->SetStat(tStat);
			CUIMgr::GetInst()->SetBossHP(tStat.HP);
		}
	}
}

void CBossScript::SaveToLevelFile(FILE* _File)
{
}

void CBossScript::LoadFromLevelFile(FILE* _File)
{
}
