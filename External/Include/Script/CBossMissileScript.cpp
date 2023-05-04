#include "pch.h"
#include "CBossMissileScript.h"
#include "CStateScript.h"
#include "CPlayerScript.h"

CBossMissileScript::CBossMissileScript()
	: CScript((UINT)SCRIPT_TYPE::BOSSMISSILESCRIPT)
	, m_MissileState((UINT)MISSILE_STATE::START)
	, m_fDelay(0.f)
	, m_bLoad(false)
	, m_bAddCollider(false)
{
}

CBossMissileScript::~CBossMissileScript()
{
}

void CBossMissileScript::begin()
{
	//if(!m_bLoad)
	//{
	//	Animator2D()->LoadAnimFromFile(GetOwner()->GetName());
	//	m_bLoad = true;
	//}
	PlayAnim();
	MeshRender()->GetDynamicMaterial();
}

void CBossMissileScript::tick()
{
	switch ((MISSILE_STATE)m_MissileState)
	{
	case MISSILE_STATE::START:
	{
		if (Animator2D()->IsFinish())
		{
			++m_MissileState;
			Animator2D()->Play(L"Missile_Projectile_Up", true);
			m_fDelay = 1.5f;
		}
	}
		break;
	case MISSILE_STATE::UP:
	{
		m_fDelay -= DT;

		Vec3 CurPos = Transform()->GetWorldPos();
		Transform()->SetRelativePos(CurPos.x, CurPos.y + 500.f * DT, CurPos.z);
		if (m_fDelay < 0.f)
		{
			Vec3 vPlyPos = CLevelMgr::GetInst()->FindObjectByName(L"Player")->Transform()->GetWorldPos();
			Transform()->SetRelativePos(vPlyPos);
			++m_MissileState;
			PlayAnim();
		}
	}
		break;
	case MISSILE_STATE::AIMING:
	{
		if (Animator2D()->IsFinish())
		{
			++m_MissileState;
			PlayAnim();
			Vec3 vPlyPos = CLevelMgr::GetInst()->FindObjectByName(L"Player")->Transform()->GetWorldPos();
			Transform()->SetRelativePos(vPlyPos.x, vPlyPos.y + 1000.f, vPlyPos.z);
			m_fDelay = 1.4f;
		}
	}
		break;
	case MISSILE_STATE::DOWN:
	{
		m_fDelay -= DT;
		Vec3 CurPos = Transform()->GetWorldPos();
		Vec3 FinalPos = Vec3(CurPos.x, CurPos.y - 700.f * DT, CurPos.z);
		Transform()->SetRelativePos(FinalPos);
		if (m_fDelay < 0.f)
		{
			++m_MissileState;
			PlayAnim();
		}
	}
		break;
	case MISSILE_STATE::EXPLOSE:
	{
		if(!m_bAddCollider)
		{
			GetOwner()->AddComponent(new CCollider2D);
			Collider2D()->SetAbsolute(true);
			Collider2D()->SetOffsetScale(Vec2(300.f, 300.f));
			m_bAddCollider = true;
		}
		if (Animator2D()->IsFinish())
		{
			Destroy();
		}
	}
		break;
	}
}

void CBossMissileScript::BeginOverlap(CCollider2D* _Other)
{
	
}

void CBossMissileScript::PlayAnim()
{
	switch (m_MissileState)
	{
	case (UINT)MISSILE_STATE::START:
		Animator2D()->Play(L"Missile_Projectile", false);
		CResMgr::GetInst()->FindRes<CSound>(L"Galmea_Rocket")->Play(1, 0.5f, true);		
		break;
	case (UINT)MISSILE_STATE::UP:
		Animator2D()->Play(L"Missile_Projectile_Up", true);
		break;
	case (UINT)MISSILE_STATE::AIMING:
		Animator2D()->Play(L"Missile_Aiming", false);
		break;
	case (UINT)MISSILE_STATE::DOWN:
		Animator2D()->Play(L"Missile_Projectile_Down", true);
		break;
	case (UINT)MISSILE_STATE::EXPLOSE:
		Animator2D()->Play(L"Missile_Bomb", false);
		CResMgr::GetInst()->FindRes<CSound>(L"Bomb")->Play(1, 1, true);
		break;
	}
}
