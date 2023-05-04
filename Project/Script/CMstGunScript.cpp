#include "pch.h"
#include "CMstGunScript.h"
#include "CMonsterScript.h"
#include "CMissileScript.h"
#include "CEffectScript.h"
#include "CStateScript.h"
#include <Engine/CAnim2D.h>
#include "CMstGunAttack.h"
#include "CMstGunIdle.h"

CMstGunScript::CMstGunScript()
	: CScript((UINT)SCRIPT_TYPE::MSTGUNSCRIPT)
	, m_pState(nullptr)
	, m_fAttackDelay(0.f)
	, m_vPlayerPos{}
	, m_vDirtoPly{}
	, m_fDirtiPly(0.f)
{
}

CMstGunScript::CMstGunScript(const CMstGunScript& _Other)
	: CScript((UINT)SCRIPT_TYPE::MSTGUNSCRIPT)
	, m_pState(nullptr)
	, m_fAttackDelay(0.f)
	, m_vPlayerPos{}
	, m_vDirtoPly{}
	, m_fDirtiPly(0.f)
{
}

CMstGunScript::~CMstGunScript()
{
}

void CMstGunScript::begin()
{
	if (nullptr == m_pState)
	{
		//Animator2D()->LoadAnimFromFile(L"Weapon");
		UINT Type = GetOwner()->GetParent()->GetScript<CMonsterScript>()->GetType();
		m_pState = GetOwner()->GetScript<CStateScript>();
		m_pState->AddState(L"Idle", new CMstGunIdle(Type));
		m_pState->AddState(L"Attack", new CMstGunAttack(Type));
		m_pState->ChangeState(L"Idle");
		m_pState->PlayAnim();
	}
}

void CMstGunScript::tick()
{
	m_vPlayerPos = GetOwner()->GetParent()->GetScript<CMonsterScript>()->GetPlayerPos();
	m_vDirtoPly = GetOwner()->GetParent()->GetScript<CMonsterScript>()->GetDirtoPly();
	m_fDirtiPly = GetOwner()->GetParent()->GetScript<CMonsterScript>()->GetDirtoPlyDegree();
	SetRelativePos();
	SetRelativeRot();
}

void CMstGunScript::SetRelativePos()
{
	// 총의 위치 설정, 플레이어를 기준으로 마우스까지 방향 * RelativeScale / 2
	Vec3 CurPos = Transform()->GetWorldPos();
	Vec3 TargetPos = m_vPlayerPos;
	Vec2 vDir = Vec2(TargetPos.x - CurPos.x, TargetPos.y - CurPos.y);
	vDir.Normalize();
	float angle = (float)acos(vDir.Dot(Vec2(0.f, -1.f)));
	if (vDir.x < 0.f)
		angle = (360.f / 180.f * XM_PI) - angle;

	// 아래를 기준으로 한 각도가 180 이상이라면 (왼쪽을 보고 있다면)
	if (angle * (180.f / XM_PI) > 180.f)
		Transform()->SetRelativePos(Vec3(vDir.x * 0.5f, vDir.y * 0.5f, 0.f));
	// 오른쪽을 보고있다면
	else
		Transform()->SetRelativePos(Vec3(-vDir.x * 0.5f, vDir.y * 0.5f, 0.f));
}

void CMstGunScript::SetRelativeRot()
{
	float fDir = m_fDirtiPly;
	// Player가 마우스 위치에 따라 좌우 반전을 하기 떄문에 그에 맞게 회전각도 조절
	if (fDir > 180.f)
	{
		// 아래를 기준으로 회전시키기 떄문에 이미지를 아래를 바라보게 -90도

		fDir += 90.f;
		fDir *= (XM_PI / 180.f);
		Transform()->SetRelativeRot(Vec3(0.f, XM_PI, fDir));
	}
	else
	{
		fDir += 90.f;
		fDir = 180.f - fDir;
		fDir *= (XM_PI / 180.f);
		Transform()->SetRelativeRot(Vec3(0.f, XM_PI, fDir));
	}
}

void CMstGunScript::Shoot()
{
	if (0.f < m_fAttackDelay)
		return;
	Animator2D()->Play(L"Magnum_fire", false);
	m_fAttackDelay = 3.f;
	Vec2 vDir = XMVector3Normalize(m_vDirtoPly);
	Vec3 PlayerPos = GetOwner()->GetParent()->GetScript<CMonsterScript>()->GetPlayerPos();
	Vec3 SpawnPos = Vec3(PlayerPos.x + vDir.x * 100, PlayerPos.y - vDir.y * 100, PlayerPos.z);

	CGameObject* Bullet = CResMgr::GetInst()->FindRes<CPrefab>(L"Bullet")->Instantiate();
	SpawnGameObject(Bullet, SpawnPos, (int)LAYER::PLAYERPROJECTILE);
	Bullet->GetScript<CMissileScript>()->SetDir(Vec3(vDir.x, -vDir.y, 0.f));
}

void CMstGunScript::SaveToLevelFile(FILE* _File)
{
}

void CMstGunScript::LoadFromLevelFile(FILE* _File)
{
	//Animator2D()->LoadAnimFromFile(L"Weapon");
	//UINT Type = GetOwner()->GetParent()->GetScript<CMonsterScript>()->GetType();
	//m_pState = GetOwner()->GetScript<CStateScript>();
	//m_pState->AddState(L"Idle", new CMstGunIdle(Type));
	//m_pState->AddState(L"Attack", new CMstGunAttack(Type));
	//m_pState->ChangeState(L"Idle");
	//m_pState->PlayAnim();
}
