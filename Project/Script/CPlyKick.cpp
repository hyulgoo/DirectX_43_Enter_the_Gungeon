#include "pch.h"
#include "CPlyKick.h"

CPlyKick::CPlyKick()
	: m_fDelay(0.f)
	, m_strAnimName{}
{
}

CPlyKick::~CPlyKick()
{
}

void CPlyKick::tick()
{
	m_fDelay -= DT;
	if (m_fDelay <= 0.f)
		GetOwnerScript()->ChangeState(L"Idle");
}

void CPlyKick::Enter()
{
	PlayAnim();
}

void CPlyKick::Exit()
{
}

void CPlyKick::PlayAnim()
{
	switch (GetDir())
	{
	case DIR::UP:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::UP], true);
		break;
	case DIR::DOWN:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::DOWN], true);
		break;
	case DIR::LEFT_UP:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::LEFT_UP], true);
		break;
	case DIR::LEFT_DOWN:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::LEFT_DOWN], true);
		break;
	case DIR::RIGHT_UP:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::RIGHT_UP], true);
		break;
	case DIR::RIGHT_DOWN:
		GetOwner()->Animator2D()->Play(m_strAnimName[(UINT)DIR::RIGHT_DOWN], true);
		break;
	}
	m_fDelay = 0.3f;
}

void CPlyKick::SetAnimName(wstring _strUp, wstring _strDown, wstring _strLeftUp, wstring _strLeftDown, wstring _strRightUp, wstring _strRightDown)
{
	m_strAnimName[(UINT)DIR::UP] = _strUp;
	m_strAnimName[(UINT)DIR::DOWN] = _strDown;
	m_strAnimName[(UINT)DIR::LEFT_UP] = _strLeftUp;
	m_strAnimName[(UINT)DIR::LEFT_DOWN] = _strLeftDown;
	m_strAnimName[(UINT)DIR::RIGHT_UP] = _strRightUp;
	m_strAnimName[(UINT)DIR::RIGHT_DOWN] = _strRightDown;
}
