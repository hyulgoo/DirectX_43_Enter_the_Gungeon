#include "pch.h"
#include "CPlayerScript.h"
#include <Engine/CEngine.h>
#include <Engine/CResMgr.h>
#include <Engine/CTexture.h>
#include <Engine/CRenderMgr.h>
#include "PlyStates.h"
#include "CStateScript.h"

#include "CWeaponScript.h"
#include "CUIMgr.h"
#include "CBossScript.h"

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pState(nullptr)
	, m_tCurWeapon()
	, m_Dir{}
	, m_Invincible_Time(0.f)
	, m_bHitEffect(false)
	, m_iBlankBullet(2)
{
	m_Dir.CurDir = DIR::DOWN;
	m_tCurWeapon = WEAPON_TYPE::OLDGUN;
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
	SetState();
	MeshRender()->GetDynamicMaterial();

}

void CPlayerScript::tick()
{
	if (0.f < m_Invincible_Time)
		m_Invincible_Time -= DT;

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

	m_Dir.PrevDir = m_Dir.CurDir;

	if (m_pState->GetStat().HP <= 0)
	{
		if(m_pState->FindState(L"Death") != m_pState->GetCurState())
			m_pState->ChangeState(L"Death");
	}

	if (KEY_TAP(KEY::V))
		m_Invincible_Time = 500.f;
	if (KEY_TAP(KEY::C))
		m_Invincible_Time = 0.f;

	CheckDir(); // 마우스와의 방향 계산
}



void CPlayerScript::BeginOverlap(CCollider2D* _Other)
{
	if (m_pState->FindState(L"Death") == m_pState->GetCurState())
		return;
	// 몬스터 및 몬스터 총알과 충돌했을 때 
	if ((int)LAYER::MONSTERPROJECTILE == _Other->GetOwner()->GetLayerIndex()
		|| (int)LAYER::MONSTER == _Other->GetOwner()->GetLayerIndex()
		|| (int)LAYER::BOMB == _Other->GetOwner()->GetLayerIndex())
	{
		if (IsInvincible())
			return;
		if (L"Galting_Gull" == _Other->GetOwner()->GetName())
		{
			if (!_Other->GetOwner()->GetScript<CBossScript>()->IsCollidable())
				return;
		}
		m_Invincible_Time = 2.f;
		tStatus stat = m_pState->GetStat();
		stat.HP -= 1;
		m_pState->SetStat(stat);
		SetHitEffect();
		CUIMgr::GetInst()->SetHP(m_pState->GetStat().HP);
		CResMgr::GetInst()->FindRes<CSound>(L"Attacked")->Play(1, 0.5f, false);
	}

	else if ((int)LAYER::FALLAREA == _Other->GetOwner()->GetLayerIndex()
		&& !IsInvincible())
	{
		m_CollisionDir = GetOwner()->Collider2D()->GetDir();
		CPlyFall* FallState = (CPlyFall*)m_pState->FindState(L"Fall"); 
		FallState->m_Dir = Collider2D()->GetDir();

		m_Invincible_Time = 3.f;

		tStatus stat = m_pState->GetStat();
		stat.HP -= 1;
		m_pState->SetStat(stat);
		SetHitEffect();

		m_pState->ChangeState(L"Fall");
		CUIMgr::GetInst()->SetHP(m_pState->GetStat().HP);
		CResMgr::GetInst()->FindRes<CSound>(L"Attacked")->Play(1, 0.5f, false);
	}
}

void CPlayerScript::OnOverlap(CCollider2D* _Other)
{
	
}

void CPlayerScript::EndOverlap(CCollider2D* _Other)
{
}

void CPlayerScript::CheckDir()
{
	if (m_pState->FindState(L"Dodge") == m_pState->GetCurState()
		|| m_pState->FindState(L"Death") == m_pState->GetCurState()
		|| m_pState->FindState(L"Fall") == m_pState->GetCurState())
		return;
	float PlytoMouseDir = GetMouseDir(Transform()->GetWorldPos()) * (180 / XM_PI); 

	float Rot = (PlytoMouseDir <= 180.f) ? 0.f : XM_PI;
	
	Transform()->SetRelativeRot(Vec3(0.f,Rot, 0.f));

	if (20.f >= PlytoMouseDir || 340.f < PlytoMouseDir)
	{
		if (DIR::DOWN != m_Dir.CurDir)
		{
			m_Dir.CurDir = DIR::DOWN;
			m_pState->GetCurState()->SetDir(DIR::DOWN);
		}
	}

	else if (20.f < PlytoMouseDir && PlytoMouseDir <= 110.f)
	{
		if (DIR::RIGHT_DOWN != m_Dir.CurDir)
		{
			m_Dir.CurDir = DIR::RIGHT_DOWN;
			m_pState->GetCurState()->SetDir(DIR::RIGHT_DOWN);
		}
	}

	else if (110.f < PlytoMouseDir && PlytoMouseDir <= 160.f)
	{
		if (DIR::RIGHT_UP != m_Dir.CurDir)
		{
			m_Dir.CurDir = DIR::RIGHT_UP;
			m_pState->GetCurState()->SetDir(DIR::RIGHT_UP);
		}
	}

	else if (160.f < PlytoMouseDir && PlytoMouseDir <= 200.f)
	{
		if (DIR::UP != m_Dir.CurDir)
		{
			m_Dir.CurDir = DIR::UP;
			m_pState->GetCurState()->SetDir(DIR::UP);
		}
	}

	else if (200.f < PlytoMouseDir && PlytoMouseDir <= 250.f)
	{
		if (DIR::LEFT_UP != m_Dir.CurDir)
		{
			m_Dir.CurDir = DIR::LEFT_UP;
			m_pState->GetCurState()->SetDir(DIR::LEFT_UP);
		}
	}

	else if (250.f < PlytoMouseDir && PlytoMouseDir <= 340.f)
	{
		if (DIR::LEFT_DOWN != m_Dir.CurDir)
		{
			m_Dir.CurDir = DIR::LEFT_DOWN;
			m_pState->GetCurState()->SetDir(DIR::LEFT_DOWN);
		}
	}	
}

void CPlayerScript::SetState()
{
	if (nullptr != m_pState)
		return;

	Animator2D()->LoadAnimFromFile(L"Player");
	m_pState = GetOwner()->GetScript<CStateScript>();
	m_pState->AddState(L"Idle", new CPlyIdle);
	m_pState->AddState(L"Walk", new CPlyWalk);
	m_pState->AddState(L"Dodge", new CPlyDodbge);
	m_pState->AddState(L"Death", new CPlyDeath);
	m_pState->AddState(L"Fall", new CPlyFall);
	m_pState->AddState(L"Kick", new CPlyKick);
	m_pState->FindState(L"Idle")->SetAnimName(L"Player_Idle_Up", L"Player_Idle_Down", L"Player_Idle_Right_Up", L"Player_Idle_Right_Down", L"Player_Idle_Right_Up", L"Player_Idle_Right_Down");
	m_pState->FindState(L"Walk")->SetAnimName(L"Player_Walk_Up", L"Player_Walk_Down", L"Player_Walk_Right_Up", L"Player_Walk_Right_Down", L"Player_Walk_Right_Up", L"Player_Walk_Right_Down");
	m_pState->FindState(L"Dodge")->SetAnimName(L"Dodge_roll_Up", L"Dodge_roll_Down", L"Dodge_roll_Right_Up", L"Dodge_roll_Right_Down", L"Dodge_roll_Right_Up", L"Dodge_roll_Right_Down");
	m_pState->FindState(L"Death")->SetAnimName(L"Death", L"Death", L"Death", L"Death", L"Death", L"Death");
	m_pState->FindState(L"Fall")->SetAnimName(L"Falling", L"Falling", L"Falling_Left", L"Falling_Left", L"Falling_Left", L"Falling_Left");
	m_pState->FindState(L"Kick")->SetAnimName(L"Player_Idle_Up", L"Kick_Down", L"Kick_Right_Up", L"Kick_Right_Down", L"Kick_Right_Up", L"Kick_Right_Down");
	m_pState->ChangeState(L"Idle");	
	m_pState->PlayAnim();

	tStatus PlayerStat;

	PlayerStat.HP = 6;
	PlayerStat.ATK = 5;
	PlayerStat.Speed = 300;

	m_pState->SetStat(PlayerStat);

	// HPUI 초기세팅
	CUIMgr::GetInst()->SetHP(m_pState->GetStat().HP);
	CUIMgr::GetInst()->SetBlank(m_iBlankBullet);
}

void CPlayerScript::SetHitEffect()
{
	int a = 2;
	MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_2, &a);
	m_bHitEffect = true;
	m_fHitEffectDelay = 1.5f;
}

void CPlayerScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
	
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_fSpeed, sizeof(float), 1, _File);
}