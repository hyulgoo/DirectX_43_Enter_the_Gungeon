#include "pch.h"
#include "CHPUIScript.h"
#include <Engine/CAnim2D.h>
#include <Engine/CEngine.h>
#include "CUIMgr.h"

CHPUIScript::CHPUIScript()
	: CScript((UINT)SCRIPT_TYPE::HPUISCRIPT)
	, m_vecHP()
	, m_vecBlank()
{
	CUIMgr::GetInst()->RegisterHPUI(this);
}

CHPUIScript::CHPUIScript(const CHPUIScript& _Other)
	: CScript((UINT)SCRIPT_TYPE::HPUISCRIPT)
	, m_vecHP()
	, m_vecBlank()
{
}

CHPUIScript::~CHPUIScript()
{
}

void CHPUIScript::begin()
{
	for (size_t i = 0; i < m_vecHP.size(); ++i)
	{
		m_vecHP[i]->SetLifeSpan(0.f);
	}
	m_vecHP.clear();
	for (size_t i = 0; i < m_vecBlank.size(); ++i)
	{
		m_vecBlank[i]->SetLifeSpan(0.f);
	}
	m_vecBlank.clear();
}

void CHPUIScript::tick()
{
}

void CHPUIScript::SetHP(int _iHP)
{
	// UI 설정할 때 level이 stop 상태면 play로 바꿔줌.
	// 모종의 이유로 play로 바로 전환하면 터짐
	// 
	if (CLevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::STOP)
		CLevelMgr::GetInst()->GetCurLevel()->ChangeState(LEVEL_STATE::PLAY);

	// 설정 시 아예 새롭게 만들어줌.
	if(!m_vecHP.empty())
	{
		for (size_t i = 0; i < m_vecHP.size(); ++i)
		{
			m_vecHP[i]->SetLifeSpan(0.f);
		}
		m_vecHP.clear();
	}

	Vec2 vResolution = CEngine::GetInst()->GetWindowResolution();
	int FullHPCount = _iHP / 2;
	int HalforEmpty = _iHP % 2;
	for (int i = 0; i < FullHPCount; ++i)
	{
		Vec3 vPos = Vec3(-vResolution.x / 2 + i * 80.f + 40.f, (vResolution.y / 2) * 0.9f, 50.f);
		CGameObject* pHP = CResMgr::GetInst()->FindRes<CPrefab>(L"HP")->Instantiate();
		SpawnGameObject(pHP, vPos, (int)LAYER::UI);
		m_vecHP.push_back(pHP);
		m_vecHP[i]->Animator2D()->Play(L"HP", true);
		m_vecHP[i]->Animator2D()->GetCurAnim()->Stop((int)HPSTATE::FULL);
	}
	int i = FullHPCount;
	if (i < 0)
		i = 0;
	for (; i < 3; ++i)
	{
		Vec3 vPos = Vec3(-vResolution.x / 2 + i * 80.f + 40.f, (vResolution.y / 2) * 0.9f, 50.f);
		CGameObject* pHP = CResMgr::GetInst()->FindRes<CPrefab>(L"HP")->Instantiate();
		SpawnGameObject(pHP, vPos, (int)LAYER::UI);
		m_vecHP.push_back(pHP);
		m_vecHP[i]->Animator2D()->Play(L"HP", true);
		if(i == FullHPCount)
		{
			pHP->Animator2D()->GetCurAnim()->Stop((int)HPSTATE::EMPTY - HalforEmpty);
		}
		else
			pHP->Animator2D()->GetCurAnim()->Stop((int)HPSTATE::EMPTY);
	}
}

void CHPUIScript::SetBlank(int _iBlankCount)
{
	if (!m_vecBlank.empty())
	m_vecBlank.clear();

	Vec2 vResolution = CEngine::GetInst()->GetWindowResolution();
	for (int i = 0; i < _iBlankCount; ++i)
	{
		Vec3 vPos = Vec3(-vResolution.x / 2 + i * 40.f + 30.f, (vResolution.y / 2) * 0.75f, 50.f);
		CGameObject* pBlank = CResMgr::GetInst()->FindRes<CPrefab>(L"Blank")->Instantiate();
		SpawnGameObject(pBlank, vPos, (int)LAYER::UI);
		m_vecBlank.push_back(pBlank);
	}
}
