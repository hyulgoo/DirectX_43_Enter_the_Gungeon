#include "pch.h"
#include "CPlyFall.h"
#include "CPlayerScript.h"
#include <Engine/CAnim2D.h>

CPlyFall::CPlyFall()
	: m_fDelay(0.f)
	, m_pWeapon(nullptr)
	, m_strAnimName{}
	, m_Dir()
{
}

CPlyFall::~CPlyFall()
{
}

void CPlyFall::tick()
{
	m_fDelay -= DT;
	if (m_fDelay <= 0.f)
	{
		GetOwnerScript()->ChangeState(L"Idle");
	}
}

void CPlyFall::Enter()
{
	if (nullptr == m_pWeapon)
	{
		vector<CGameObject*> vecChild = GetOwner()->GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			if (L"Weapon" == vecChild[i]->GetName())
				m_pWeapon = vecChild[i];
		}
	}
	CResMgr::GetInst()->FindRes<CSound>(L"Fall")->Play(1);
	PlayAnim();
	m_Dir = GetOwner()->Collider2D()->GetDir(); 
	m_pWeapon->Transform()->SetRelativeScale(0.f, 0.f, 0.f);
}

void CPlyFall::Exit()
{
	Vec3 CurPos = GetOwner()->Transform()->GetWorldPos();
	Vec2 SpawnPos = {};
	switch (GetOwner()->GetScript<CPlayerScript>()->GetCollisionDir())
	{
	case COLLISION_DIR::UP:
		SpawnPos = Vec2(0.f, -48.f);
		break;
	case COLLISION_DIR::DOWN:
		SpawnPos = Vec2(0.f, 48.f);
		break;
	case COLLISION_DIR::LEFT:
		SpawnPos = Vec2(48.f, 0.f);
		break;
	case COLLISION_DIR::RIGHT:
		SpawnPos = Vec2(-48.f, 0.f);
		break;
	}
	GetOwner()->Transform()->SetRelativePos(CurPos.x + SpawnPos.x, CurPos.y + SpawnPos.y, CurPos.z);
	GetOwner()->Animator2D()->GetCurAnim()->Reset();
	m_pWeapon->Transform()->SetRelativeScale(1.f, 1.f, 1.f);
}

void CPlyFall::PlayAnim()
{
	switch (GetDir())
	{
	case DIR::UP:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::UP], false);
		break;
	case DIR::DOWN:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::DOWN], false);
		break;
	case DIR::LEFT_UP:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::LEFT_UP], false);
		break;
	case DIR::LEFT_DOWN:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::LEFT_DOWN], false);
		break;
	case DIR::RIGHT_UP:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::RIGHT_UP], false);
		break;
	case DIR::RIGHT_DOWN:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::RIGHT_DOWN], false);
		break;
	}
	m_fDelay = 2.f;
}

void CPlyFall::SetAnimName(wstring _strUp, wstring _strDown, wstring _strLeftUp, wstring _strLeftDown, wstring _strRightUp, wstring _strRightDown)
{
	m_strAnimName[(UINT)DIR::UP] = _strUp;
	m_strAnimName[(UINT)DIR::DOWN] = _strDown;
	m_strAnimName[(UINT)DIR::LEFT_UP] = _strLeftUp;
	m_strAnimName[(UINT)DIR::LEFT_DOWN] = _strLeftDown;
	m_strAnimName[(UINT)DIR::RIGHT_UP] = _strRightUp;
	m_strAnimName[(UINT)DIR::RIGHT_DOWN] = _strRightDown;
}
