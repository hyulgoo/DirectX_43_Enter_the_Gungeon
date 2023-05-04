#include "pch.h"
#include "CPlyWalk.h"
#include "CPlyDodbge.h"

CPlyWalk::CPlyWalk()
	: m_strAnimName{}
	, m_MoveStop{}
	, m_OtherCollider{}
	, m_fDelay(0.f)
	, m_pWalkEffect(nullptr)
	, m_fSoundDelay(0.f)
{
	for (int i = 0; i < 4; ++i)
	{
		m_MoveStop[i] = true;
	}
}

CPlyWalk::~CPlyWalk()
{
}

void CPlyWalk::tick()
{
	Move();

	if (Vec3(0.f, 0.f, 0.f) == GetOwner()->Transform()->GetWorldPos() - GetOwner()->Transform()->GetPrevPos())
		m_fDelay += DT;
	if(m_fDelay >= 0.1f)
		ChangeState(L"Idle");

	if (KEY_PRESSED(KEY::RBTN)
		|| GetOwnerScript()->FindState(L"Dodge") == GetOwnerScript()->GetCurState())
		ChangeState(L"Dodge");
}

void CPlyWalk::Enter()
{
	if (nullptr == m_pWalkEffect)
		m_pWalkEffect = CLevelMgr::GetInst()->FindObjectByName(L"WalkEffect");
	m_pWalkEffect->ParticleSystem()->SetModuleData(PARTICLE::PLAYER_WALK);
	m_pWalkEffect->ParticleSystem()->OnOff(true);
	CResMgr::GetInst()->FindRes<CSound>(L"Player_Walk")->Play(0);
	PlayAnim();
}

void CPlyWalk::Exit()
{
	m_fDelay = 0.f;
	m_pWalkEffect->ParticleSystem()->OnOff(false);
	CResMgr::GetInst()->FindRes<CSound>(L"Player_Walk")->Stop();
	if (!m_OtherCollider.empty())
	{
		if (GetOwnerScript()->GetCurState() == GetOwnerScript()->FindState(L"Dodge"))
		{
			CPlyDodbge* Dodge = (CPlyDodbge*)GetOwnerScript()->FindState(L"Dodge");
			Dodge->CheckCollision((COLLISION_DIR)m_OtherCollider.begin()->second);
		}
	}
}

void CPlyWalk::PlayAnim()
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

void CPlyWalk::SetAnimName(wstring _strUp, wstring _strDown, wstring _strLeftUp, wstring _strLeftDown, wstring _strRightUp, wstring _strRightDown)
{
	m_strAnimName[(UINT)DIR::UP] = _strUp;
	m_strAnimName[(UINT)DIR::DOWN] = _strDown;
	m_strAnimName[(UINT)DIR::LEFT_UP] = _strLeftUp;
	m_strAnimName[(UINT)DIR::LEFT_DOWN] = _strLeftDown;
	m_strAnimName[(UINT)DIR::RIGHT_UP] = _strRightUp;
	m_strAnimName[(UINT)DIR::RIGHT_DOWN] = _strRightDown;
}

void CPlyWalk::Move()
{
	Vec3 vCurPos = GetOwner()->Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W) || KEY_TAP(KEY::W))
	{
		if (m_MoveStop[0])
			vCurPos.y += DT * GetOwnerScript()->GetStat().Speed;
	}

	if (KEY_PRESSED(KEY::S) || KEY_TAP(KEY::S))
	{
		if (m_MoveStop[1])
			vCurPos.y -= DT * GetOwnerScript()->GetStat().Speed;
	}

	if (KEY_PRESSED(KEY::A) || KEY_TAP(KEY::A))
	{
		if (m_MoveStop[2])
			vCurPos.x -= DT * GetOwnerScript()->GetStat().Speed;
	}

	if (KEY_PRESSED(KEY::D) || KEY_TAP(KEY::D))
	{
		if (m_MoveStop[3])
			vCurPos.x += DT * GetOwnerScript()->GetStat().Speed;
	}
	GetOwner()->Transform()->SetRelativePos(vCurPos);
}

void CPlyWalk::BeginOverlap(CCollider2D* _Other)
{
	if (!_Other->IsPushable() && (int)LAYER::WALL == _Other->GetOwner()->GetLayerIndex())
	{
		Vec3 Pos = GetOwner()->Transform()->GetWorldPos();
		Vec2 CollisionArea = GetOwner()->Collider2D()->GetCollisionArea();
		GetOwner()->Transform()->SetRelativePos(Vec3(Pos.x + CollisionArea.x, Pos.y + CollisionArea.y, Pos.z));
		COLLISION_DIR Dir = GetOwner()->Collider2D()->GetDir();
		UINT OtherID = _Other->GetOwner()->GetID();
		m_MoveStop[(UINT)Dir] = false;
		m_OtherCollider.insert(make_pair(OtherID, (UINT)Dir));
	}
}

void CPlyWalk::OnOverlap(CCollider2D* _Other)
{
}

void CPlyWalk::EndOverlap(CCollider2D* _Other)
{
	if (!_Other->IsPushable() && (int)LAYER::WALL == _Other->GetOwner()->GetLayerIndex())
	{
		if (m_OtherCollider.empty())
			return;
		UINT OtherID = _Other->GetOwner()->GetID();
		int a = 0;
		for (const auto& pair : m_OtherCollider)
		{
			if (pair.first == OtherID)
				a = 1;
		}
		if (0 == a)
			return;
		m_MoveStop[m_OtherCollider.find(OtherID)->second] = true;
		m_OtherCollider.erase(OtherID);
	}
}
