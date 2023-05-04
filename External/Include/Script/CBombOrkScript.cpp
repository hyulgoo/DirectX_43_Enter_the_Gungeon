#include "pch.h"
#include "CBombOrkScript.h"
#include <Engine/CAnim2D.h>

CBombOrkScript::CBombOrkScript()
	: CScript((UINT)SCRIPT_TYPE::BOMBORKSCRIPT)
	, m_strAnimName(L"Ork_Bomb")
{
	SetName(L"Ork");
}

CBombOrkScript::~CBombOrkScript()
{
}
void CBombOrkScript::begin()
{
	Animator2D()->LoadAnimFromFile(GetName());
	Animator2D()->Play(m_strAnimName, false);
	Animator2D()->GetCurAnim()->Stop(0);
}

void CBombOrkScript::tick()
{
	if (Animator2D()->GetCurAnim()->IsFinish())
	{
		CGameObject* pExplosion = CResMgr::GetInst()->FindRes<CPrefab>(L"Explosion")->Instantiate();
		SpawnGameObject(pExplosion, Transform()->GetWorldPos(), (int)LAYER::MONSTERPROJECTILE);
		SetLifeSpan(0.f);
	}
}

void CBombOrkScript::BeginOverlap(CCollider2D* _Other)
{
	Animator2D()->Play(m_strAnimName, false);
	Animator2D()->GetCurAnim()->PlayStop();
	GetOwner()->DeleteComponent((UINT)COMPONENT_TYPE::COLLIDER2D);
}

void CBombOrkScript::SaveToLevelFile(FILE* _File)
{
	SaveWString(m_strAnimName, _File);
}

void CBombOrkScript::LoadFromLevelFile(FILE* _File)
{
	LoadWString(m_strAnimName, _File);
}

