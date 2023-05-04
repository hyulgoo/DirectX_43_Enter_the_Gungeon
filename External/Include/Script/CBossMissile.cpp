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
		// �߻� ����� �����ٸ� �̻����� �ڽ��� �������� �߻���. 
		// �߻� �� �̻����� ȭ�� �ٱ����� ���� �� ���� �ð� �� ���� �� �������� �̻��Ϸ� ����.
		// �̻����� �浹���� �ʰ� �̻����� ������ �� �߻��ϴ� ���� �ִϸ��̼� ���� ���� �浹ü ����.
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
