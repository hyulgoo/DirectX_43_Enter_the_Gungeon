#include "pch.h"
#include "CMonsterScript.h"
#include "MstStates.h"
#include "CStateScript.h"
#include "CMstBookAttack.h"
#include "CMissileScript.h"


CMonsterScript::CMonsterScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT)
	, m_MstType()
	, m_iRoomNum(-1)
	, m_pState(nullptr)
	, m_Dir{}
	, m_pPlayer(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fLengthtoPly, "Length");
}

CMonsterScript::CMonsterScript(const CMonsterScript& _Other)
	: CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT)
	, m_MstType(_Other.m_MstType)
	, m_pState(nullptr)
	, m_Dir{}
	, m_pPlayer(nullptr)
{
	if (nullptr == m_pPlayer)
		m_pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::begin()
{
	MeshRender()->GetDynamicMaterial();
	if (nullptr == m_pPlayer)
	{
		m_pPlayer = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	}
	// 하위 Status에게 타입 알려주기
	wstring strName = GetOwner()->GetName();
	if (L"RedBook" == strName)
		m_MstType = (UINT)MONSTER_TYPE::REDBOOK;
	else if(L"GreenBook" == strName)
		m_MstType = (UINT)MONSTER_TYPE::GREENBOOK;
	else if (L"BulletKin" == strName)
		m_MstType = (UINT)MONSTER_TYPE::BULLETKIN;
	else if (L"ShotgunKin_Blue" == strName)
		m_MstType = (UINT)MONSTER_TYPE::SHOTGUNKIN_BLUE;
	else if (L"ShotgunKin_Red" == strName)
		m_MstType = (UINT)MONSTER_TYPE::SHOTGUNKIN_RED;
	else if (L"Cubulon" == strName)
		m_MstType = (UINT)MONSTER_TYPE::CUBULON;
	else if (L"Gigi" == strName)
		m_MstType = (UINT)MONSTER_TYPE::GIGI;
	else if (L"ShotBat" == strName)
		m_MstType = (UINT)MONSTER_TYPE::SHOTBAT;
	else if (L"BulletBat" == strName)
		m_MstType = (UINT)MONSTER_TYPE::BULLETBAT;
	else if (L"Mage" == strName)
		m_MstType = (UINT)MONSTER_TYPE::MAGE;
	SetState();
}

void CMonsterScript::tick()
{
	if (0.f < m_fAttackDelay)
		m_fAttackDelay -= DT;
	// hp가 0 아래라면 Dead
	if (0.f >= m_pState->GetStat().HP)
	{
		int a = 0;
		MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_2, &a);
		m_pState->ChangeState(L"Dead");
		m_bHitEffect = false;
	}

	if (m_bHitEffect)
	{
		if (m_fHitEffectDelay < 0.f)
		{
			int a = 0;
			MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_2, &a);
			m_bHitEffect = false;
			return;
		}
		m_fHitEffectDelay -= DT;
	}
	CheckDir();
}

void CMonsterScript::CheckDir()
{
	if (m_pState->FindState(L"Attak") == m_pState->GetCurState()
		|| m_pState->FindState(L"Dead") == m_pState->GetCurState()
		|| m_pState->FindState(L"Fall") == m_pState->GetCurState())
		return;
	// 플레이어와의 거리, 방향, 각도를 계산함.
	Vec3 MstPos = Transform()->GetWorldPos();
	m_vPlayerPos = m_pPlayer->Transform()->GetWorldPos();

	m_fAngle = GetDirfloat(MstPos, m_vPlayerPos, true);
	Vec2 MontoPly = Vec2(m_vPlayerPos.x - MstPos.x, m_vPlayerPos.y - MstPos.y);
	m_fLengthtoPly = MontoPly.Length();
	m_vDirToPly = Vec2(m_vPlayerPos.x - MstPos.x, m_vPlayerPos.y - MstPos.y);
	m_vDirToPly.Normalize();


	float Rot = 0.f;
	// 일부 몬스터는 좌우를 반대로
	if(m_MstType == (UINT)MONSTER_TYPE::BULLETKIN || m_MstType == (UINT)MONSTER_TYPE::SHOTGUNKIN_BLUE
		|| m_MstType == (UINT)MONSTER_TYPE::SHOTGUNKIN_RED || m_MstType == (UINT)MONSTER_TYPE::MAGE)
		Rot = (m_fAngle <= 180.f ? XM_PI : 0.f);
	else
		Rot = (m_fAngle <= 180.f ? 0.f : XM_PI);

	Transform()->SetRelativeRot(Vec3(0.f, Rot, 0.f));

	if (20.f >= m_fAngle || 340.f < m_fAngle)
	{
		if (DIR::DOWN != m_Dir.CurDir)
		{
			m_Dir.CurDir = DIR::DOWN;
			m_pState->GetCurState()->SetDir(DIR::DOWN);
		}
	}

	else if (20.f < m_fAngle && m_fAngle <= 90.f)
	{
		if (DIR::RIGHT_DOWN != m_Dir.CurDir)
		{
			m_Dir.CurDir = DIR::RIGHT_DOWN;
			m_pState->GetCurState()->SetDir(DIR::RIGHT_DOWN);
		}
	}

	else if (90.f < m_fAngle && m_fAngle <= 160.f)
	{
		if (DIR::RIGHT_UP != m_Dir.CurDir)
		{
			m_Dir.CurDir = DIR::RIGHT_UP;
			m_pState->GetCurState()->SetDir(DIR::RIGHT_UP);
		}
	}

	else if (160.f < m_fAngle && m_fAngle <= 200.f)
	{
		if (DIR::UP != m_Dir.CurDir)
		{
			m_Dir.CurDir = DIR::UP;
			m_pState->GetCurState()->SetDir(DIR::UP);
		}
	}

	else if (200.f < m_fAngle && m_fAngle <= 270.f)
	{
		if (DIR::LEFT_UP != m_Dir.CurDir)
		{
			m_Dir.CurDir = DIR::LEFT_UP;
			m_pState->GetCurState()->SetDir(DIR::LEFT_UP);
		}
	}

	else if (270.f < m_fAngle && m_fAngle <= 340.f)
	{
		if (DIR::LEFT_DOWN != m_Dir.CurDir)
		{
			m_Dir.CurDir = DIR::LEFT_DOWN;
			m_pState->GetCurState()->SetDir(DIR::LEFT_DOWN);
		}
	}
}

void CMonsterScript::SetState()
{
	if (m_pState != nullptr)
		return;
	m_pState = GetOwner()->GetScript<CStateScript>();
	if ((UINT)MONSTER_TYPE::REDBOOK == m_MstType
		|| (UINT)MONSTER_TYPE::GREENBOOK == m_MstType)
		m_pState->AddState(L"Attack", new CMstBookAttack(m_MstType));
	else if ((UINT)MONSTER_TYPE::BULLETKIN == m_MstType
		|| (UINT)MONSTER_TYPE::SHOTGUNKIN_BLUE == m_MstType
		|| (UINT)MONSTER_TYPE::SHOTGUNKIN_RED == m_MstType)
		m_pState->AddState(L"Attack", new CMstKinAttack(m_MstType));
	else if ((UINT)MONSTER_TYPE::BULLETBAT == m_MstType
		|| (UINT)MONSTER_TYPE::SHOTBAT == m_MstType)
		m_pState->AddState(L"Attack", new CMstBatAttack(m_MstType));

	m_pState->AddState(L"Idle", new CMstIdle(m_MstType));
	m_pState->AddState(L"Trace",new CMstChase(m_MstType));
	m_pState->AddState(L"Dead", new CMstDead(m_MstType));
	m_pState->ChangeState(L"Idle");
	m_pState->PlayAnim();
}

void CMonsterScript::SetHitEffect()
{
	int a = 1;
	MeshRender()->GetDynamicMaterial()->SetScalarParam(INT_2, &a);
	m_bHitEffect = true;
	m_fHitEffectDelay = 1.f;
}

void CMonsterScript::BeginOverlap(CCollider2D* _Other)
{
	if ((int)LAYER::PLAYERPROJECTILE == _Other->GetOwner()->GetLayerIndex())
	{
		int iDamage = _Other->GetOwner()->GetScript<CMissileScript>()->GetDamage();
		tStatus CurStat = m_pState->GetStat();
		CurStat.HP -= iDamage;
		m_pState->SetStat(CurStat);
	}
	if ((int)LAYER::BOMB == _Other->GetOwner()->GetLayerIndex())
	{
		tStatus CurStat = m_pState->GetStat();
		CurStat.HP -= 20;
		m_pState->SetStat(CurStat);
	}

	if ((int)LAYER::FALLAREA == _Other->GetOwner()->GetLayerIndex())
		m_pState->ChangeState(L"Fall");
	if ((int)LAYER::WALL == _Other->GetOwner()->GetLayerIndex())
	{
		Vec2 Area = Collider2D()->GetCollisionArea();
		Vec3 CurPos = Transform()->GetWorldPos();
		Transform()->SetRelativePos(Vec3(CurPos.x + Area.x + 1, CurPos.y + Area.y + 1, CurPos.z));
	}
}

void CMonsterScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_MstType, sizeof(UINT), 1, _File);
}

void CMonsterScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_MstType, sizeof(UINT), 1, _File);
	m_pState = nullptr;
	m_pPlayer = nullptr;
	SetState();
}
