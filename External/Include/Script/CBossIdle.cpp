#include "pch.h"
#include "CBossIdle.h"
#include <Engine/CAnim2D.h>
#include "CBossScript.h"
#include "CUIMgr.h"

CBossIdle::CBossIdle()
	: m_bShout(false)
	, m_fDelay(3.f)
	, m_bShowIntro(false)
{
}

CBossIdle::~CBossIdle()
{
}

void CBossIdle::tick()
{
	if (m_bShout)
	{
		if (2 == GetOwner()->Animator2D()->GetCurAnim()->GetCurFrmNum())
			GetOwner()->Animator2D()->GetCurAnim()->PlayStop();
		if (!m_bShowIntro)
		{
			CGameObject* IntroSceen = CResMgr::GetInst()->FindRes<CPrefab>(L"BossIntro")->Instantiate();
			IntroSceen->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"BOSS_INTRO"));
			IntroSceen->MeshRender()->GetDynamicMaterial();
			IntroSceen->SetLifeSpan(2.9f);
			SpawnGameObject(IntroSceen, Vec3(0.f, 0.f, 1.f), (int)LAYER::UI);
			CResMgr::GetInst()->FindRes<CSound>(L"BossIntro")->Play(1);
			m_bShowIntro = true;
		}
		m_fDelay -= DT;
	}
	// 하늘에서 떨어지는 모션 후 미사일 패턴의 2프레임에서 고정하여 카메라 쉐이크 및 확대효과로 소리치는 효과를 줄 것
	else if(GetOwner()->Animator2D()->IsFinish())
	{
		GetOwner()->Animator2D()->Play(L"Boss_Attack_Missile", false);
		CUIMgr::GetInst()->BossAppear();
		CUIMgr::GetInst()->SetBossHP(GetOwnerScript()->GetStat().HP);
		m_bShout = true;
	}

	if (m_fDelay < 0.f)
		ChangeState(L"Trace");
}

void CBossIdle::Enter()
{
	GetOwner()->GetScript<CBossScript>()->SetCollidable(false);
	GetOwner()->Animator2D()->Play(L"Boss_TakeDown_Down", false);
	GetOwner()->GetScript<CBossScript>()->SetRotable(false);
	CGameObject* BossHPUI = CResMgr::GetInst()->FindRes<CPrefab>(L"BossHP")->Instantiate();
	SpawnGameObject(BossHPUI, Vec3(0.f, 0.f, 0.f), (int)LAYER::DEFAULT);
}

void CBossIdle::Exit()
{
	CResMgr::GetInst()->FindRes<CSound>(L"BGM_Game")->Stop();
	CResMgr::GetInst()->FindRes<CSound>(L"BGM_Boss")->Play(0, 0.2f, false);
	GetOwner()->GetScript<CBossScript>()->SetRotable(true);
	GetOwner()->GetScript<CBossScript>()->SetCollidable(true);
}
