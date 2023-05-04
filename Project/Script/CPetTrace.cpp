#include "pch.h"
#include "CPetTrace.h"
#include "CPetScript.h"
#include <Engine/CAstarMgr.h>

CPetTrace::CPetTrace()
	: m_fDelay(0.f)
	, m_strAnimName{}
{
}

CPetTrace::~CPetTrace()
{
}

void CPetTrace::tick()
{
	// Player���� �Ÿ��� 150 �Ʒ���� Idle ���·� ��ȯ 
	if (120.f >= GetOwner()->GetScript<CPetScript>()->GetLengthtoPly())
		ChangeState(L"Idle");

	m_fDelay -= DT;

	Vec3 Pos = GetOwner()->Transform()->GetWorldPos();
	Vec3 PlayerPos = GetOwner()->GetScript<CPetScript>()->GetPlayerPos();
	// A*�� Player���� ���� ���� Ÿ���� ã�� �������� ���� 
	if(0.f >= m_fDelay)
	{
		m_DestPos = CAstarMgr::GetInst()->SetFind(Pos, PlayerPos);
		m_fDelay = 0.1f;
	}
	// ������ ������ ���� * ���ǵ� * DT�� ƽ �� �̵���Ŵ
	Vec2 DestDir = Vec2(m_DestPos.x - Pos.x, m_DestPos.y - Pos.y);
	DestDir.Normalize();
	Vec2 vDir = DestDir;
	Vec3 FinalPos = Vec3(Pos.x + vDir.x * GetOwnerScript()->GetStat().Speed * DT
		, Pos.y + vDir.y * GetOwnerScript()->GetStat().Speed * DT, Pos.z);
	GetOwner()->Transform()->SetRelativePos(FinalPos);
}

void CPetTrace::Enter()
{
	PlayAnim();
	m_fDelay = 0.1f;
}

void CPetTrace::Exit()
{
}

void CPetTrace::SetAnimName(wstring _strUp, wstring _strDown, wstring _strLeftUp, wstring _strLeftDown, wstring _strRightUp, wstring _strRightDown)
{
	m_strAnimName[(UINT)DIR::UP] = _strUp;
	m_strAnimName[(UINT)DIR::DOWN] = _strDown;
	m_strAnimName[(UINT)DIR::LEFT_UP] = _strLeftUp;
	m_strAnimName[(UINT)DIR::LEFT_DOWN] = _strLeftDown;
	m_strAnimName[(UINT)DIR::RIGHT_UP] = _strRightUp;
	m_strAnimName[(UINT)DIR::RIGHT_DOWN] = _strRightDown;
}

void CPetTrace::PlayAnim()
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
}
