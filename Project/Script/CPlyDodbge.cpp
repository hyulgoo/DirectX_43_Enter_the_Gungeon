#include "pch.h"
#include "CPlyDodbge.h"
#include "CPlayerScript.h"
#include "CPlyWalk.h"

CPlyDodbge::CPlyDodbge()
	: m_fDelay(0.f)
	, m_strAnimName{}
	, m_fSpeed()
	, m_vDir()
	, m_pWalkEffect(nullptr)
	, m_DIR()
	, m_pWeapon(nullptr)
{
}

CPlyDodbge::~CPlyDodbge()
{
}

void CPlyDodbge::tick()
{
		
	m_fDelay -= DT;
	if (m_fDelay <= 0.f)
		GetOwnerScript()->ChangeState(L"Idle");

	Vec3 CurPos = GetOwner()->Transform()->GetRelativePos();	
	GetOwner()->Transform()->SetRelativePos(Vec3(CurPos.x + m_vDir.x * m_fSpeed * DT, CurPos.y + m_vDir.y * m_fSpeed * DT, CurPos.z));
}

void CPlyDodbge::Enter()
{
	if(nullptr == m_pWeapon)
	{
		vector<CGameObject*> vecChild = GetOwner()->GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			if (L"Weapon" == vecChild[i]->GetName())
				m_pWeapon = vecChild[i];
		}
	}
	m_pWeapon->Transform()->SetRelativeScale(0.f, 0.f, 0.f);
	if (nullptr == m_pWalkEffect)
		m_pWalkEffect = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"WalkEffect");
	m_pWalkEffect->ParticleSystem()->OnOff(true);
	m_pWalkEffect->ParticleSystem()->SetModuleData(PARTICLE::PLAYER_JUMP);
	CResMgr::GetInst()->FindRes<CSound>(L"Player_Jump")->Play(1, 1.f, true);
	m_fSpeed = GetOwnerScript()->GetStat().Speed * 1.4f;
	if (KEY_PRESSED(KEY::W))
	{
		if (KEY_PRESSED(KEY::A))
		{
			m_vDir = Vec2(-1.f, 1.f);
			m_DIR = DIR::LEFT_UP;
		}
		else if (KEY_PRESSED(KEY::D))
		{
			m_vDir = Vec2(1.f, 1.f);
			m_DIR = DIR::RIGHT_UP;
		}
		else
		{
			m_vDir = Vec2(0.f, 1.f);
			m_DIR = DIR::UP;
		}
	}
	else if (KEY_PRESSED(KEY::S))
	{
		if (KEY_PRESSED(KEY::A))
		{
			m_vDir = Vec2(-1.f, -1.f);
			m_DIR = DIR::LEFT_DOWN;
		}
		else if (KEY_PRESSED(KEY::D))
		{
			m_vDir = Vec2(1.f, -1.f);
			m_DIR = DIR::RIGHT_DOWN;
		}
		else
		{
			m_vDir = Vec2(0.f, -1.f);
			m_DIR = DIR::DOWN;
		}
	}
	else if (KEY_PRESSED(KEY::A))
	{
		m_vDir = Vec2(-1.f, 0.f);
		m_DIR = DIR::LEFT_DOWN;
	}
	else if (KEY_PRESSED(KEY::D))
	{
		m_vDir = Vec2(1.f, 0.f);
		m_DIR = DIR::RIGHT_DOWN;
	}
	PlayAnim();
	m_vDir.Normalize();
	CGameObject* pasd = GetOwner();
	GetOwner()->GetScript<CPlayerScript>()->SetInvincibleTime(0.675f);
}

void CPlyDodbge::Exit()
{
	if (m_bCollision)
	{
		Vec2 CollsionArea = GetOwner()->Collider2D()->GetCollisionArea();
		Vec3 CurPos = GetOwner()->Transform()->GetWorldPos();
		switch (m_CollisionDir)
		{
		case COLLISION_DIR::UP:
			GetOwner()->Transform()->SetRelativePos(Vec3(CurPos.x, CurPos.y + CollsionArea.y * 1.1f, CurPos.z));
			break;
		case COLLISION_DIR::DOWN:
			GetOwner()->Transform()->SetRelativePos(Vec3(CurPos.x, CurPos.y + CollsionArea.y * 1.1f, CurPos.z));
			break;
		case COLLISION_DIR::LEFT:
			GetOwner()->Transform()->SetRelativePos(Vec3(CurPos.x + CollsionArea.x * 1.1f, CurPos.y, CurPos.z));
			break;
		case COLLISION_DIR::RIGHT:
			GetOwner()->Transform()->SetRelativePos(Vec3(CurPos.x + CollsionArea.x * 1.1f, CurPos.y, CurPos.z));
			break;
		}
		m_bCollision = false;
	}
	m_pWalkEffect->ParticleSystem()->OnOff(false);
	m_pWalkEffect->ParticleSystem()->SetModuleData(PARTICLE::PLAYER_WALK);
	m_pWeapon->Transform()->SetRelativeScale(1.f, 1.f, 1.f);
	CPlyWalk* pWalkState = (CPlyWalk*)GetOwnerScript()->FindState(L"Walk");
	pWalkState->Clear();
}

void CPlyDodbge::PlayAnim()
{
	if (DIR::LEFT_DOWN == m_DIR || DIR::LEFT_UP == m_DIR)
		GetOwner()->Transform()->SetRelativeRot(0.f, XM_PI, 0.f);
	else
		GetOwner()->Transform()->SetRelativeRot(0.f, 0.f, 0.f);

	switch (m_DIR)
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
	m_fDelay = 0.675f;
}

void CPlyDodbge::CheckCollision(COLLISION_DIR _Dir)
{
	switch (_Dir)
	{
	case COLLISION_DIR::UP:
		if (m_vDir.y > 0.f)
			m_vDir.y = 0.f;
		break;
	case COLLISION_DIR::DOWN:
		if (m_vDir.y < 0.f)
			m_vDir.y = 0.f;
		break;
	case COLLISION_DIR::LEFT:
		if (m_vDir.x < 0.f)
			m_vDir.x = 0.f;
		break;
	case COLLISION_DIR::RIGHT:
		if (m_vDir.x > 0.f)
			m_vDir.x = 0.f;
		break;
	}
}

void CPlyDodbge::SetAnimName(wstring _strUp, wstring _strDown, wstring _strLeftUp, wstring _strLeftDown, wstring _strRightUp, wstring _strRightDown)
{
	m_strAnimName[(UINT)DIR::UP] = _strUp;
	m_strAnimName[(UINT)DIR::DOWN] = _strDown;
	m_strAnimName[(UINT)DIR::LEFT_UP] = _strLeftUp;
	m_strAnimName[(UINT)DIR::LEFT_DOWN] = _strLeftDown;
	m_strAnimName[(UINT)DIR::RIGHT_UP] = _strRightUp;
	m_strAnimName[(UINT)DIR::RIGHT_DOWN] = _strRightDown;
}

void CPlyDodbge::BeginOverlap(CCollider2D* _Other)
{
	if((int)LAYER::WALL == _Other->GetOwner()->GetLayerIndex())
	{
		m_bCollision = true;
		m_CollisionDir = GetOwner()->Collider2D()->GetDir();
		Vec2 CollsionArea = GetOwner()->Collider2D()->GetCollisionArea();
		Vec3 CurPos = GetOwner()->Transform()->GetWorldPos();
		switch (m_CollisionDir)
		{
		case COLLISION_DIR::UP:
			if (m_vDir.y > 0.f)
				m_vDir.y = 0.f;
			break;
		case COLLISION_DIR::DOWN:
			if (m_vDir.y < 0.f)
				m_vDir.y = 0.f;
			break;
		case COLLISION_DIR::LEFT:
			if (m_vDir.x < 0.f)
				m_vDir.x = 0.f;
			break;
		case COLLISION_DIR::RIGHT:
			if (m_vDir.x > 0.f)
				m_vDir.x = 0.f;
			break;
		}
	}
}

void CPlyDodbge::EndOverlap(CCollider2D* _Other)
{
	if ((int)LAYER::WALL == _Other->GetOwner()->GetLayerIndex())
	{
		if (m_bCollision)
		{
			switch (m_CollisionDir)
			{
			case COLLISION_DIR::UP:
				if (DIR::UP == m_DIR || DIR::LEFT_UP == m_DIR || DIR::RIGHT_UP == m_DIR)
					m_vDir.y = 1.f;
				break;
			case COLLISION_DIR::DOWN:
				if (DIR::DOWN == m_DIR || DIR::LEFT_DOWN == m_DIR || DIR::RIGHT_DOWN == m_DIR)
					m_vDir.y = -1.f;
				break;
			case COLLISION_DIR::LEFT:
				if (DIR::LEFT == m_DIR || DIR::LEFT_DOWN == m_DIR || DIR::LEFT_UP == m_DIR)
					m_vDir.x = -1.f;
				break;
			case COLLISION_DIR::RIGHT:
				if (DIR::RIGHT == m_DIR || DIR::RIGHT_DOWN == m_DIR || DIR::RIGHT_UP == m_DIR)
					m_vDir.x = 1.f;
				break;
			}
			m_bCollision = false;
		}
	}
}
