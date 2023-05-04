#include "pch.h"
#include "CBlankScript.h"

CBlankScript::CBlankScript()
	: CScript((UINT)BLANKSCRIPT)
{
}

CBlankScript::~CBlankScript()
{
}

void CBlankScript::begin()
{
	GetOwner()->SetLifeSpan(m_fDelay);
}

void CBlankScript::tick()
{
	Vec3 vScale = Transform()->GetRelativeScale();
	vScale = Vec3(vScale.x + (1280.f / m_fDelay) * DT, vScale.y + (1280.f / m_fDelay) * DT, vScale.z);
	Transform()->SetRelativeScale(vScale);
}

void CBlankScript::BeginOverlap(CCollider2D* _Other)
{
	if ((int)LAYER::MONSTERPROJECTILE == _Other->GetOwner()->GetLayerIndex())
		_Other->GetOwner()->SetLifeSpan(0.f);
}
