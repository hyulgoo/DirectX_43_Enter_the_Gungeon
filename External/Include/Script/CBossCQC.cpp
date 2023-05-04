#include "pch.h"
#include "CBossCQC.h"
#include "CBossScript.h"

CBossCQC::CBossCQC()
	: m_fDelay(0.8f)
	, m_bAttack(false)
{
}

CBossCQC::~CBossCQC()
{
}

void CBossCQC::tick()
{
	m_fDelay -= DT;
	if(m_fDelay < 0.f)
	{
		if (m_bAttack)
		{
			if (GetOwner()->Animator2D()->IsFinish())
				ChangeState(L"Trace");
		}
		else
		{
			CGameObject* pCQC = new CGameObject;
			pCQC->AddComponent(new CTransform);
			pCQC->AddComponent(new CCollider2D);

			pCQC->Transform()->SetRelativePos(0.f, 0.f, 0.f);
			pCQC->Collider2D()->SetOffsetScale(Vec2(1.2f, 1.2f));

			pCQC->SetLifeSpan(0.4f);
			SpawnGameObject(pCQC, Vec3(GetOwner()->Transform()->GetWorldPos()), (int)LAYER::MONSTERPROJECTILE);
			m_bAttack = true;
		}
	}
}

void CBossCQC::Enter()
{
	PlayAnim();
}

void CBossCQC::Exit()
{
}

void CBossCQC::PlayAnim()
{
	float Dir = GetOwner()->GetScript<CBossScript>()->GetDirtoPlyDegree();

	float Rot = Dir < 180 ? 0.f : XM_PI;

	GetOwner()->Transform()->SetRelativeRot(Vec3(0.f, Rot, 0.f));
	GetOwner()->Animator2D()->Play(L"Boss_Attack_CQC.", false);
}
