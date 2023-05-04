#include "pch.h"
#include "CBossMissile.h"
#include <Engine/CAnim2D.h>
#include "CBossScript.h"

CBossMissile::CBossMissile()
	: m_iMissileCount(0)
{
}

CBossMissile::~CBossMissile()
{
}

void CBossMissile::tick()
{
	if (10 == m_iMissileCount)
	{
		ChangeState(L"Trace");
	}
	else if (GetOwner()->Animator2D()->GetCurAnim()->IsFinish() && m_iMissileCount != 10)
	{
		m_iMissileCount += 1;
		// 발사 모션이 끝났다면 미사일을 자신의 위쪽으로 발사함. 
		// 발사 후 미사일은 화면 바깥까지 나간 후 일정 시간 후 조준 후 내려오는 미사일로 변함.
		// 미사일은 충돌하지 않고 미사일이 내려온 후 발생하는 폭발 애니메이션 때만 폭발 충돌체 생성.
		GetOwner()->Animator2D()->GetCurAnim()->Reset();
		CGameObject* pMissile = CResMgr::GetInst()->FindRes<CPrefab>(L"Boss_Missile")->Instantiate();
		Vec3 CurPos = GetOwner()->Transform()->GetWorldPos();
		SpawnGameObject(pMissile, Vec3(CurPos.x - 10.f, CurPos.y + 130.f, CurPos.z - 10.f), (int)LAYER::MONSTERPROJECTILE);
	}
}

void CBossMissile::Enter()
{
	PlayAnim();
	CResMgr::GetInst()->FindRes<CSound>(L"Galmea_GLoding")->Play(1);
	GetOwner()->GetScript<CBossScript>()->SetRotable(false);
}

void CBossMissile::Exit()
{
	GetOwner()->GetScript<CBossScript>()->SetRotable(true);
}

void CBossMissile::PlayAnim()
{
	GetOwner()->Animator2D()->Play(L"Boss_Attack_Missile", false);
}
