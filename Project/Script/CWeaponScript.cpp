#include "pch.h"
#include "CWeaponScript.h"

#include <Engine/CAnim2D.h>
#include <Engine/CEngine.h>
#include <Engine/CRenderMgr.h>
#include "CStateScript.h"
#include "WpStates.h"
#include "CMissileScript.h"
#include "CEffectScript.h"
#include "CFireLightScript.h"
#include "CUIMgr.h"

CWeaponScript::CWeaponScript()
	: CScript((UINT)SCRIPT_TYPE::WEAPONSCRIPT)
	, m_pState(nullptr)
	, m_fDir(0.f)
	, m_Type(WEAPON_TYPE::OLDGUN)
	, m_ParentPos{}
	, m_AttackDelay(0.f)
	, m_CurBulletCount(0)
	, m_MaxBulletCount(0)
	, m_iMOA(0)
	, m_pFireEffect(nullptr)
	, m_pLightEffect(nullptr)
	, m_BulletDir(0.f)
{
}

CWeaponScript::~CWeaponScript()
{
}

void CWeaponScript::begin()
{
	MeshRender()->GetDynamicMaterial();
	CUIMgr::GetInst()->SetWeapon(m_Type);
	CUIMgr::GetInst()->PlayWeaponAnim(WEAPON_STATES::IDLE);
	// StateScript �ʱ� ����
	if(nullptr == m_pState)
	{
		Animator2D()->LoadAnimFromFile(L"Weapon");
		m_pState = GetOwner()->GetScript<CStateScript>();
		m_pState->AddState(L"Idle", new CWpIdle);
		m_pState->AddState(L"Attack", new CWpAttack);
		m_pState->AddState(L"Reload", new CWpReload);
		m_pState->ChangeState(L"Idle");
		m_pState->SetWeaponType(m_Type);
		ChangeWeapon(WEAPON_TYPE::OLDGUN);
	}

	// ȭ�� ����Ʈ �ʱ� ����
	if (nullptr == m_pFireEffect)
	{
		vector<CGameObject*> vecChild = GetOwner()->GetParent()->GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			if (L"Effect" == vecChild[i]->GetName())
			{
				m_pFireEffect = vecChild[i];
				break;
			}
			
		}
	}

	// ���� ����Ʈ �ʱ� ����
	if (nullptr == m_pLightEffect)
	{
		vector<CGameObject*> vecChild = GetOwner()->GetParent()->GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			if (L"LightEffect" == vecChild[i]->GetName())
			{
				m_pLightEffect = vecChild[i];
				break;
			}

		}
	}
	m_pState->PlayAnim();
}

void CWeaponScript::tick()
{
	m_AttackDelay -= DT;

	// �����̰� 0 �Ʒ��� �� ��Ŭ���� �ϸ� ���
	// �Ѿ� ������ 0�̶�� ����������
	if (m_AttackDelay <= 0.f)
	{
		if(KEY_TAP(KEY::LBTN) || KEY_PRESSED(KEY::LBTN))
		{
			if (m_CurBulletCount == 0)
				Reload();
			else if (m_pState->FindState(L"Reload") != m_pState->GetCurState())
				Shoot();
		}
	}

	if (KEY_TAP(KEY::R) && m_CurBulletCount != m_MaxBulletCount)
	{
		Reload();
	}

	if (KEY_TAP(KEY::_1))
	{
		ChangeWeapon(WEAPON_TYPE::OLDGUN);
	}
	if (KEY_TAP(KEY::_2))
	{
		ChangeWeapon(WEAPON_TYPE::CROSSBOW);
	}

	m_ParentPos = GetOwner()->GetParent()->Transform()->GetWorldPos();

	SetRelativePos();
	SetRelativeRot();
}

void CWeaponScript::SetRelativePos()
{
	// ���� ��ġ ����, �÷��̾ �������� ���콺���� ���� * RelativeScale / 2
	Vec3 CurPos = Transform()->GetWorldPos();
	Vec3 TargetPos = m_ParentPos;
	Vec3 Cursor = CRenderMgr::GetInst()->GetRay();
	Vec2 vDir = Vec2(Cursor.x - TargetPos.x, Cursor.y - TargetPos.y);
	vDir.Normalize();
	float angle = (float)acos(vDir.Dot(Vec2(0.f, -1.f)));
	if (vDir.x < 0.f)
		angle = (360.f / 180.f * XM_PI) - angle;

	m_BulletDir = angle;

	// �Ʒ��� �������� �� ������ 180 �̻��̶�� (������ ���� �ִٸ�)
	if (angle * (180.f / XM_PI) > 180.f)
		Transform()->SetRelativePos(Vec3(-vDir.x * 0.4f, vDir.y * 0.4f, 5.f));
	// �������� �����ִٸ�
	else
		Transform()->SetRelativePos(Vec3(vDir.x * 0.4f, vDir.y * 0.4f, 5.f));
}

void CWeaponScript::SetRelativeRot()
{
	// ���� ���� ����
	m_fDir = GetMouseDir(m_ParentPos) * (180.f / XM_PI);
	
	// Player�� ���콺 ��ġ�� ���� �¿� ������ �ϱ� ������ �׿� �°� ȸ������ ����
	if (m_fDir > 180.f)
	{
		// �Ʒ��� �������� ȸ����Ű�� ������ �̹����� �Ʒ��� �ٶ󺸰� -90��
		m_fDir -= 90.f;
		m_fDir = 180.f - m_fDir;
		m_fDir *= (XM_PI / 180.f);
		Transform()->SetRelativeRot(Vec3(0.f, 0.f, m_fDir));
	}
	else
	{
		m_fDir -= 90.f;
		m_fDir *= (XM_PI / 180.f);
		Transform()->SetRelativeRot(Vec3(0.f, 0.f, m_fDir));
	}
}

void CWeaponScript::ChangeWeapon(WEAPON_TYPE _Weapon)
{
	// ���� ���� ����
	m_Type = _Weapon;
	m_pState->SetWeaponType(m_Type);
	m_pState->PlayAnim();
	switch (_Weapon)
	{
	case WEAPON_TYPE::OLDGUN:
	{
		m_MaxBulletCount = 6;
		m_iMOA = 25;
	}
		break;
	case WEAPON_TYPE::CROSSBOW:
	{
		m_MaxBulletCount = 1;
		m_iMOA = 1;
	}
		break;
	case WEAPON_TYPE::MAGNUM:
	{
		m_MaxBulletCount = 6;
		m_iMOA = 10;
	}
		break;
	}
	m_CurBulletCount = m_MaxBulletCount;
	CUIMgr::GetInst()->SetWeapon(m_Type);
	CUIMgr::GetInst()->PlayWeaponAnim(WEAPON_STATES::IDLE);
	CResMgr::GetInst()->FindRes<CSound>(L"WeaponPickup")->Play(1, 1.f, true);
}

void CWeaponScript::Shoot()
{
	// �Ѿ� ������ ���̰� �Ѿ� ��ġ �� ������ ����Ͽ� �Ѿ� ����
	--m_CurBulletCount;
	Vec3 Cursor = CRenderMgr::GetInst()->GetRay();
	Vec2 vDir = Vec2(Cursor.x - m_ParentPos.x, -Cursor.y - m_ParentPos.y);
	vDir = Vec2(vDir.x, vDir.y + m_ParentPos.y * 2);
	vDir.Normalize();
	Vec3 SpawnPos = Vec3(m_ParentPos.x + vDir.x * 80, m_ParentPos.y - vDir.y * 80, m_ParentPos.z);
	wstring BulletName;
	switch (m_Type)
	{
	case WEAPON_TYPE::OLDGUN:
		BulletName = L"Bullet";
		m_AttackDelay = 0.2f;
		break;
	case WEAPON_TYPE::CROSSBOW:
		BulletName = L"Crossbow";
		m_AttackDelay = 0.75f;
		break;
	case WEAPON_TYPE::MAGNUM:
		BulletName = L"Bullet";
		m_AttackDelay = 0.15f;
		break;
	}
	float RandomAngle = (float)((rand() * 33) % m_iMOA);
	RandomAngle /= 57.3f;
	float x = acos(vDir.x);
	float y = asin(-vDir.y);
	x += RandomAngle - (float)((m_iMOA / 2)) / 57.3f;
	y += RandomAngle - (float)((m_iMOA / 2)) / 57.3f;
	Vec2 BulletDir = Vec2(cos(x), sin(y));
	CGameObject* Bullet = CResMgr::GetInst()->FindRes<CPrefab>(BulletName)->Instantiate();
	Bullet->MeshRender()->GetDynamicMaterial();
	Bullet->GetScript<CMissileScript>()->SetDir(Vec3(BulletDir.x, BulletDir.y, 0.f));
	Bullet->GetScript<CMissileScript>()->SetRot(m_BulletDir);
	Bullet->GetScript<CMissileScript>()->SetSpeed(1200.f);
	switch (m_Type)
	{
	case WEAPON_TYPE::OLDGUN:
		Bullet->GetScript<CMissileScript>()->SetDamage(6);
		break;
	case WEAPON_TYPE::CROSSBOW:
		Bullet->GetScript<CMissileScript>()->SetDamage(22);
		break;
	case WEAPON_TYPE::MAGNUM:
		Bullet->GetScript<CMissileScript>()->SetDamage(50);
		break;
	}
	SpawnGameObject(Bullet, SpawnPos, (int)LAYER::PLAYERPROJECTILE);
	
	m_pFireEffect->GetScript<CEffectScript>()->SetEffect(true);
	m_pLightEffect->GetScript<CFireLightScript>()->SetLight(true);
	

	m_pState->ChangeState(L"Attack");
}

void CWeaponScript::Reload()
{
	m_pState->ChangeState(L"Reload");
	m_CurBulletCount = m_MaxBulletCount;
}
