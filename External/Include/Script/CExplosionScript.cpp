#include "pch.h"
#include "CExplosionScript.h"

CExplosionScript::CExplosionScript()
	: CScript((UINT)SCRIPT_TYPE::EXPLOSIONSCRIPT)
	, m_strAnimName{}
	, m_bBomb(false)
{
}

CExplosionScript::~CExplosionScript()
{
}

void CExplosionScript::begin()
{
	SetLifeSpan(ParticleSystem()->GetModuleData().MaxLifeTime);
	Collider2D()->SetAbsolute(true);
	Collider2D()->SetOffsetScale(Vec2(250.f, 250.f));
}

void CExplosionScript::tick()
{
}

void CExplosionScript::BeginOverlap(CCollider2D* _Other)
{
}

void CExplosionScript::SaveToLevelFile(FILE* _File)
{
	SaveWString(m_strAnimName, _File);
}

void CExplosionScript::LoadFromLevelFile(FILE* _File)
{
	LoadWString(m_strAnimName, _File);
}
