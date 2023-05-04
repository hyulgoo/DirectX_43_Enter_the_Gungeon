#include "pch.h"
#include "CMstBatAttack.h"
#include <Engine/CAnim2D.h>
#include "CMissileScript.h"
#include "CMonsterScript.h"

CMstBatAttack::CMstBatAttack(UINT _MstType)
	: m_MstType(_MstType)
	, m_strAnimName{}
	, m_fDelay(0.f)
	, m_bAttack(0.f)
	, m_fAccTime(0.f)
	, m_iBulletCount(0)

{
	SetAnimName();
}

CMstBatAttack::~CMstBatAttack()
{
}

void CMstBatAttack::tick()
{
	if (0 < m_fDelay)
	{
		m_fDelay -= DT;
		return;
	}
	if (!m_bAttack)
	{
		Vec3 CurPos = GetOwner()->Transform()->GetWorldPos();
		Vec3 TargetPos = CLevelMgr::GetInst()->FindObjectByName(L"Player")->Transform()->GetWorldPos();
		m_vDir = Vec3(TargetPos.x - CurPos.x, TargetPos.y - CurPos.y, 1.f);
		m_vDir.Normalize();
		m_bAttack = true;
		CResMgr::GetInst()->FindRes<CSound>(L"BulletKingThrow")->Play(1);
	}
	Vec3 CurPos = GetOwner()->Transform()->GetWorldPos();
	Vec2 Diff = Vec2(m_vDir.x * 700.f * DT, m_vDir.y * 700.f * DT);
	GetOwner()->Transform()->SetRelativePos(CurPos.x + Diff.x, CurPos.y + Diff.y, CurPos.z);
	
	if(GetOwner()->Animator2D()->IsFinish())
	{
		if ((UINT)MONSTER_TYPE::BULLETBAT == GetOwner()->GetScript<CMonsterScript>()->GetType())
			GetOwner()->Animator2D()->Play(L"BulletBat_Attacking", true);
		else
			GetOwner()->Animator2D()->Play(L"ShotBat_Attacking", true);
	}

}

void CMstBatAttack::Enter()
{
	tStatus Stat = GetOwnerScript()->GetStat();
	m_fDelay = 0.572f;
	GetOwner()->Animator2D()->Play(m_strAnimName, false);
}

void CMstBatAttack::Exit()
{
	GetOwner()->Animator2D()->GetCurAnim()->Reset();
	tStatus Stat = GetOwnerScript()->GetStat();
	m_bAttack = false;
	m_iBulletCount = 0;
	m_fAccTime = 0.f;
}

void CMstBatAttack::SetAnimName()
{
	switch ((MONSTER_TYPE)m_MstType)
	{
	case MONSTER_TYPE::SHOTBAT:
	{
		m_strAnimName= L"ShotBat_Attack";
	}
		break;
	case MONSTER_TYPE::BULLETBAT:
	{
		m_strAnimName = L"BulletBat_Attack";
	}
		break;
	}
}

void CMstBatAttack::BeginOverlap(CCollider2D* _Other)
{
	if((int)LAYER::WALL == _Other->GetOwner()->GetLayerIndex()
		|| (int)LAYER::PLAYER == _Other->GetOwner()->GetLayerIndex()
		|| (int)LAYER::PLAYERPROJECTILE == _Other->GetOwner()->GetLayerIndex())
	{
		ChangeState(L"Dead");
	}
}
