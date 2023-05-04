#include "pch.h"
#include "CPlyDeath.h"
#include "CPlayerScript.h"

CPlyDeath::CPlyDeath()
	: m_fDelay(0.f)
	, m_strAnimName{}
	, m_pWeapon(nullptr)
{
}

CPlyDeath::~CPlyDeath()
{
}

void CPlyDeath::tick()
{
	if (KEY_TAP(KEY::I))
	{
		tStatus Stat = GetOwner()->GetScript<CStateScript>()->GetStat();
		Stat.HP = 6;
		GetOwner()->GetScript<CStateScript>()->SetStat(Stat);
		ChangeState(L"Idle");
	}

}

void CPlyDeath::Enter()
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
	m_pWeapon->Transform()->SetRelativeScale(0.f, 0.f, 0.f);
	PlayAnim();
	CResMgr::GetInst()->FindRes<CSound>(L"Death")->Play(1);
	GetOwner()->GetScript<CPlayerScript>()->SetInvincibleTime(200.f);
}

void CPlyDeath::Exit()
{
	m_pWeapon->Transform()->SetRelativeScale(1.f, 1.f, 1.f);
	GetOwner()->GetScript<CPlayerScript>()->SetInvincibleTime(0.f);
}

void CPlyDeath::SetAnimName(wstring _strUp, wstring _strDown, wstring _strLeftUp, wstring _strLeftDown, wstring _strRightUp, wstring _strRightDown)
{
	m_strAnimName[(UINT)DIR::UP] = _strUp;
	m_strAnimName[(UINT)DIR::DOWN] = _strDown;
	m_strAnimName[(UINT)DIR::LEFT_UP] = _strLeftUp;
	m_strAnimName[(UINT)DIR::LEFT_DOWN] = _strLeftDown;
	m_strAnimName[(UINT)DIR::RIGHT_UP] = _strRightUp;
	m_strAnimName[(UINT)DIR::RIGHT_DOWN] = _strRightDown;
}

void CPlyDeath::PlayAnim()
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
