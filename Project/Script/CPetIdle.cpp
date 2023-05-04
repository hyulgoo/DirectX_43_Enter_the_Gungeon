#include "pch.h"
#include "CPetIdle.h"
#include "CPetScript.h"

CPetIdle::CPetIdle()
	: m_strAnimName{}
{
}

CPetIdle::~CPetIdle()
{
}

void CPetIdle::tick()
{
	if (120.f < GetOwner()->GetScript<CPetScript>()->GetLengthtoPly())
		ChangeState(L"Trace");
}

void CPetIdle::Enter()
{
	PlayAnim();
}

void CPetIdle::Exit()
{
}

void CPetIdle::SetAnimName(wstring _strUp, wstring _strDown, wstring _strLeftUp, wstring _strLeftDown, wstring _strRightUp, wstring _strRightDown)
{
	m_strAnimName[(UINT)DIR::UP] = _strUp;
	m_strAnimName[(UINT)DIR::DOWN] = _strDown;
	m_strAnimName[(UINT)DIR::LEFT_UP] = _strLeftUp;
	m_strAnimName[(UINT)DIR::LEFT_DOWN] = _strLeftDown;
	m_strAnimName[(UINT)DIR::RIGHT_UP] = _strRightUp;
	m_strAnimName[(UINT)DIR::RIGHT_DOWN] = _strRightDown;
}

void CPetIdle::PlayAnim()
{
	CAnimator2D* Anim = GetOwner()->Animator2D();
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
}
