#include "pch.h"
#include "CBossJump.h"
#include "CBossScript.h"
#include <Engine/CAnim2D.h>

CBossJump::CBossJump()
	: m_ATKState(JUMP_PATTERN::UP)
	, m_vDir{}
	, m_fDelay(0.f)
	, m_bSpawnEffect(false)
{
}

CBossJump::~CBossJump()
{
}

void CBossJump::tick()
{
	m_fDelay -= DT;

	switch (m_ATKState)
	{
	case JUMP_PATTERN::UP:
	{
		if (0.f > m_fDelay)
		{
			// �ö󰡴� �߿��� Anim �ܿ� �ƹ� �͵� ���� ����.
			// Anim ����� �����ٸ� Warning���� ��ȯ
			m_ATKState = JUMP_PATTERN::WARNING;
			CResMgr::GetInst()->FindRes<CSound>(L"Galmea_Melee")->Play(1);
			PlayAnim();
			m_fDelay = 3.f;
			GetOwner()->GetScript<CBossScript>()->SetCollidable(false);
			return;
		}
	}
		break;
	case JUMP_PATTERN::WARNING:
	{
		// �÷��̾ ����ٴϴ� �׸���
		if (0.f > m_fDelay)
		{
			// �׸��� ����
			vector<CGameObject*> vecChild = GetOwner()->GetChild();
			for (size_t i = 0; i < vecChild.size(); ++i)
			{
				if (L"ShadowParticle" == vecChild[i]->GetName())
				{
					vecChild[i]->Transform()->SetRelativeScale(0.f, 0.f, 0.f);
				}
			}
			
			// �����̰� ������ ���� �������� ���鼭 ����Ʈ ����
			m_ATKState = JUMP_PATTERN::DOWN;
			m_fDelay = 0.35f;
			PlayAnim();
			CGameObject* pEffect = new CGameObject;
			pEffect->AddComponent(new CTransform);
			pEffect->AddComponent(new CCollider2D);
			pEffect->AddComponent(new CParticleSystem(PARTICLE::BOSS_JUMP));

			pEffect->Transform()->SetRelativeScale(350.f, 350.f, 1.f);
			pEffect->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
			pEffect->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
			pEffect->SetLifeSpan(1.f);
			Vec3 vBossPos = GetOwner()->Transform()->GetWorldPos();
			SpawnGameObject(pEffect, vBossPos, (int)LAYER::DEFAULT);
		}
		if( 50.f < GetOwner()->GetScript<CBossScript>()->GetLengthtoPly())
		{
			Vec3 CurPos = GetOwner()->Transform()->GetWorldPos();
			m_vDir = GetOwner()->GetScript<CBossScript>()->GetDirtoPly();
			Vec2 Diff = Vec2(m_vDir.x * 270.f * DT, m_vDir.y * 270.f * DT);
			GetOwner()->Transform()->SetRelativePos(CurPos.x + Diff.x, CurPos.y + Diff.y, CurPos.z);
		}
	}
		break;
	case JUMP_PATTERN::DOWN:
	{
		if (!m_bSpawnEffect)
		{
			// �׸��� ����
			vector<CGameObject*> vecChild = GetOwner()->GetChild();
			for (size_t i = 0; i < vecChild.size(); ++i)
			{
				if (L"ShadowParticle" == vecChild[i]->GetName())
				{
					vecChild[i]->Transform()->SetRelativeScale(250.f, 250.f, 1.f);
				}
			}
			CResMgr::GetInst()->FindRes<CSound>(L"GunDeadGun_Shot")->Play(1);
			m_bSpawnEffect = true;
			GetOwner()->GetScript<CBossScript>()->SetCollidable(true);
		}
		// �������鼭 ����
		if (0.f > m_fDelay)
		{

			if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish())
				ChangeState(L"Trace");
		}
	}
		break;
	}
}

void CBossJump::Enter()
{
	PlayAnim();
	m_fDelay = 1.f;
	CResMgr::GetInst()->FindRes<CSound>(L"Galmea_Jump")->Play(1);
	GetOwner()->GetScript<CBossScript>()->SetRotable(false);
}

void CBossJump::Exit()
{
	m_ATKState = JUMP_PATTERN::UP;
	m_bSpawnEffect = false;
	GetOwner()->GetScript<CBossScript>()->SetRotable(true);
}

void CBossJump::PlayAnim()
{
	switch (m_ATKState)
	{
	case JUMP_PATTERN::UP:
		GetOwner()->Animator2D()->Play(L"Boss_TakeDown_Up", false);
		break;
	case JUMP_PATTERN::WARNING:
		GetOwner()->Animator2D()->Play(L"Boss_TakeDown_Warning", false);
		break;
	case JUMP_PATTERN::DOWN:
		GetOwner()->Animator2D()->Play(L"Boss_TakeDown_Down", false);
		break;
	}
}
