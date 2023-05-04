#include "pch.h"
#include "CBlankScript.h"

CBlankScript::CBlankScript()
	: CScript((UINT)BLANKSCRIPT)
	, m_fDelay(1.f)
	, m_WaveTime(0.f)
	, m_bDeleteCollider(false)
{
}

CBlankScript::~CBlankScript()
{
}

void CBlankScript::begin()
{
	MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"RTCopyTex"));
	MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, &m_WaveTime);
	MeshRender()->GetDynamicMaterial();
	Transform()->SetRelativeScale(Vec3(1280.f, 768.f, 1.f));
	Collider2D()->SetAbsolute(true);
	Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));
	Collider2D()->SetOffsetScale(Vec2(1280.f, 768.f));
	GetOwner()->SetLifeSpan(m_fDelay);
}

void CBlankScript::tick()
{
	m_WaveTime += DT;
	MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, &m_WaveTime);
	MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"RTCopyTex"));

	if (!m_bDeleteCollider && m_WaveTime > 0.2f)
	{
		GetOwner()->DeleteComponent((UINT)COMPONENT_TYPE::COLLIDER2D);
		m_bDeleteCollider = true;
	}

}

void CBlankScript::BeginOverlap(CCollider2D* _Other)
{
}
