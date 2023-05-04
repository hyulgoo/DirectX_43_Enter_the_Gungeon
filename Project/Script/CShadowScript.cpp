#include "pch.h"
#include "CShadowScript.h"

CShadowScript::CShadowScript()
	: CScript((UINT)SCRIPT_TYPE::SHADOWSCRIPT)
	, m_pTarget(nullptr)
	, m_fYPos(-0.4)
{
}

CShadowScript::~CShadowScript()
{
}

void CShadowScript::begin()
{
	ParticleSystem()->SetFollow();
	if (nullptr == m_pTarget)
		m_pTarget = GetOwner()->GetParent();
	Transform()->SetRelativePos(Vec3(0.f, m_fYPos, 0.f));
}

void CShadowScript::tick()
{
}