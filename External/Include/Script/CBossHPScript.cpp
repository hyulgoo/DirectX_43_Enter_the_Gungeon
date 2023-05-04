#include "pch.h"
#include "CBossHPScript.h"
#include "CUIMgr.h"

CBossHPScript::CBossHPScript()
	: CScript((UINT)BOSSHPSCRIPT)
	, m_pBossHPBar(nullptr)
	, m_pBossHPFrame(nullptr)
{
}

CBossHPScript::~CBossHPScript()
{
}

void CBossHPScript::begin()
{
	CUIMgr::GetInst()->RegisterBossUI(this);
}

void CBossHPScript::tick()
{
}

void CBossHPScript::SetBossHP(int _iHP)
{
	// HPbar의 크기를 보스 HP에 따라 조정
	// 보스 MAX_HP를 기준으로 비율계산
	float HPRatio = (float)_iHP / 300;
	float HpBarLength = 495.f * HPRatio;
	Vec2 HPBarLtp = Vec2(-248.f, -330.f);
	Vec2 HPBarBtm = Vec2(-248.f + HpBarLength, -350.f);
	Vec3 HPBarScale = Vec3(HPBarBtm.x - HPBarLtp.x, 20.f, 1.f);
	Vec3 HPBarPos = Vec3(HPBarLtp.x + HPBarScale.x / 2, HPBarLtp.y - HPBarScale.y / 2, 40.f);

	m_pBossHPBar->Transform()->SetRelativePos(HPBarPos);
	m_pBossHPBar->Transform()->SetRelativeScale(HPBarScale);
}

void CBossHPScript::BossAppear()
{
	// 보스 등장 시 보스스크립트에서 호출하여 등록
	if (nullptr == m_pBossHPBar)
	{
		m_pBossHPBar = CResMgr::GetInst()->FindRes<CPrefab>(L"BossHPbar")->Instantiate();
		m_pBossHPBar->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"BOSSHP_BAR"));
		m_pBossHPBar->MeshRender()->GetDynamicMaterial();
		SpawnGameObject(m_pBossHPBar, Vec3(0.f, -340.f, 40.f), (int)LAYER::UI);
	}
	if (nullptr == m_pBossHPFrame)
	{
		m_pBossHPFrame = CResMgr::GetInst()->FindRes<CPrefab>(L"BossHPUI")->Instantiate();
		m_pBossHPFrame->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"BOSSHP_GUIDE"));
		m_pBossHPFrame->MeshRender()->GetDynamicMaterial();
		SpawnGameObject(m_pBossHPFrame, Vec3(0.f, -340.f, 50.f), (int)LAYER::UI);
	}
}
