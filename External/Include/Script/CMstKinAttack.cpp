#include "pch.h"
#include "CMstKinAttack.h"
#include "CMonsterScript.h"
#include <Engine/CAstarMgr.h>

CMstKinAttack::CMstKinAttack(UINT _MstType)
	: m_MstType(_MstType)
	, m_pWeapon(nullptr)
{
}

CMstKinAttack::CMstKinAttack(const CMstKinAttack& _Other)
	: m_MstType(_Other.m_MstType)
	, m_pWeapon(nullptr)
{
}

CMstKinAttack::~CMstKinAttack()
{
}

void CMstKinAttack::tick()
{
	Vec3 Pos = GetOwner()->Transform()->GetWorldPos();
	Vec3 PlayerPos = GetOwner()->GetScript<CMonsterScript>()->GetPlayerPos();

	// A*�� Player���� ���� ���� Ÿ���� ã�� �������� ���� 
	Vec2 DestPos = CAstarMgr::GetInst()->SetFind(Pos, PlayerPos);

	if (0.f == DestPos.x && 0.f == DestPos.y)
		return;

	// ������ ������ ���� * ���ǵ� * DT�� ƽ �� �̵���Ŵ
	Vec2 DestDir = Vec2(DestPos.x - Pos.x, DestPos.y - Pos.y);
	DestDir.Normalize();
	Vec2 vDir = DestDir;
	Vec3 FinalPos = Vec3(Pos.x + vDir.x * GetOwnerScript()->GetStat().Speed / 2 * DT
		, Pos.y + vDir.y * GetOwnerScript()->GetStat().Speed / 2 * DT, Pos.z);
	GetOwner()->Transform()->SetRelativePos(FinalPos);
}

void CMstKinAttack::Enter()
{
	if (nullptr == m_pWeapon)
	{
		vector<CGameObject*> vecChild = GetOwner()->GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			if (L"Weapon" == vecChild[i]->GetName())
			{
				m_pWeapon = vecChild[i];
				break;
			}			
		}
	}
	m_pWeapon->GetScript<CStateScript>()->ChangeState(L"Attack");
}

void CMstKinAttack::Exit()
{
}

void CMstKinAttack::PlayAnim()
{
}

void CMstKinAttack::Shoot()
{
}
