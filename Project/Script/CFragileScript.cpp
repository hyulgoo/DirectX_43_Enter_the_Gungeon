#include "pch.h"
#include "CFragileScript.h"
#include <Engine/CAnim2D.h>
#include <Engine/CRenderMgr.h>
#include "CGameCameraScript.h"

CFragileScript::CFragileScript()
	: CScript((UINT)SCRIPT_TYPE::FRAGILESCRIPT)
	, m_strAnimName{}
	, m_bHit(false)
	, m_bBomb(false)
{
}

CFragileScript::CFragileScript(const CFragileScript& _Other)
	: CScript((UINT)SCRIPT_TYPE::FRAGILESCRIPT)
	, m_strAnimName(_Other.m_strAnimName)
	, m_bHit(false)
	, m_bBomb(false)
{
}

CFragileScript::~CFragileScript()
{
}

void CFragileScript::begin()
{
	if (L"Ork_Bomb" == m_strAnimName)
		m_bBomb = true;
	//Animator2D()->LoadAnimFromFile(GetOwner()->GetName());
	Animator2D()->Play(m_strAnimName, false);
	Animator2D()->GetCurAnim()->Stop(0);
}

void CFragileScript::tick()
{
	if (m_bBomb)
	{
		if (m_bHit)
		{
			if(Animator2D()->IsFinish())
			{
				CGameObject* pBomb = CResMgr::GetInst()->FindRes<CPrefab>(L"Bomb")->Instantiate();
				pBomb->Animator2D()->Play(L"Bomb", true);
				pBomb->SetLifeSpan(1.f);
				pBomb->Transform()->SetRelativeScale(362.f, 284.f, 1.f);
				pBomb->Collider2D()->SetAbsolute(true);
				pBomb->Collider2D()->SetOffsetScale(Vec2(250.f, 250.f));
				Vec3 CurPos = Transform()->GetWorldPos();
				Vec3 SpawnPos = Vec3(CurPos.x + 10.f, CurPos.y, CurPos.z);
				SpawnGameObject(pBomb, SpawnPos, (int)LAYER::BOMB);
				GetOwner()->SetLifeSpan(0.f);
				m_bBomb = false;
				m_bHit = false;
				CResMgr::GetInst()->FindRes<CSound>(L"Bomb")->Play(1, 1.f, true);
				CRenderMgr::GetInst()->GetMainCam()->GetOwner()->GetScript< CGameCameraScript>()->CameraShake(10.f, 1000.f, 0.2f);
			}
		}
	}	
	else if (m_bHit)
	{
		int a = rand() % 3;
		switch (a)
		{
		case 0:
			CResMgr::GetInst()->FindRes<CSound>(L"barrel_break_01")->Play(1, 0.5f, true);
			break;
		case 1:
			CResMgr::GetInst()->FindRes<CSound>(L"barrel_break_02")->Play(1, 0.5f, true);
			break;
		case 2:
			CResMgr::GetInst()->FindRes<CSound>(L"barrel_break_03")->Play(1, 0.5f, true);
			break;
		}
		GetOwner()->DeleteComponent((UINT)COMPONENT_TYPE::COLLIDER2D);
		m_bHit = false;
	}
}

void CFragileScript::BeginOverlap(CCollider2D* _Other)
{
	if(!m_bHit)
	{
		if ((int)LAYER::PLAYER == _Other->GetOwner()->GetLayerIndex()
			|| (int)LAYER::MONSTER == _Other->GetOwner()->GetLayerIndex()
			|| (int)LAYER::PLAYERPROJECTILE == _Other->GetOwner()->GetLayerIndex()
			|| (int)LAYER::MONSTERPROJECTILE == _Other->GetOwner()->GetLayerIndex()
			|| (int)LAYER::BOMB == _Other->GetOwner()->GetLayerIndex())
		{
			Animator2D()->GetCurAnim()->PlayStop();
			m_bHit = true;
			Vec3 CurPos = Transform()->GetWorldPos();
			Transform()->SetRelativePos(CurPos.x, CurPos.y, CurPos.z + 150.f);
		}
	}
}

void CFragileScript::SaveToLevelFile(FILE* _File)
{
	SaveWString(m_strAnimName, _File);
}

void CFragileScript::LoadFromLevelFile(FILE* _File)
{
	LoadWString(m_strAnimName, _File);
}
