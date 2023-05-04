#include "pch.h"
#include "CPlayButtonUIScript.h"
#include <Engine/CRenderMgr.h>
#include <Engine/CAnim2D.h>
#include <Engine/CEventMgr.h>
#include <Engine/CCollisionMgr.h>
#include "CUIMgr.h"

CPlayButtonUIScript::CPlayButtonUIScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYBUTTONUISCRIPT)
	, m_pPlayButton(nullptr)
	, m_bMouseOn(false)
{
}

CPlayButtonUIScript::~CPlayButtonUIScript()
{
}

void CPlayButtonUIScript::begin()
{
	if(nullptr == m_pPlayButton)
	{
		m_pPlayButton = CResMgr::GetInst()->FindRes<CPrefab>(L"PlayButton")->Instantiate();
		SpawnGameObject(m_pPlayButton, Vec3(-530.f, -300.f, 1.f), (int)LAYER::UI);

		CGameObject* pMain = CResMgr::GetInst()->FindRes<CPrefab>(L"Main")->Instantiate();
		SpawnGameObject(pMain, Vec3(0.f, 0.f, 5.f), (int)LAYER::DEFAULT);
		pMain->Animator2D()->Play(L"Main", true);
		CResMgr::GetInst()->FindRes<CSound>(L"MenuBGM")->Play(0, 1.f);

		CGameObject* pLogo = CResMgr::GetInst()->FindRes<CPrefab>(L"Logo")->Instantiate();
		pLogo->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"TITLE"));
		pLogo->MeshRender()->GetDynamicMaterial();
		SpawnGameObject(pLogo, Vec3(0.f, 100.f, 4.f), (int)LAYER::UI);
	}
}

void CPlayButtonUIScript::tick()
{
	Vec3 CursorPos = CRenderMgr::GetInst()->GetRay();
	Vec3 BtnPos = m_pPlayButton->Transform()->GetWorldPos();
	Vec3 BtnScale = m_pPlayButton->Transform()->GetRelativeScale();
	Vec2 BtnLtp = Vec2(BtnPos.x - BtnScale.x, BtnPos.y + BtnScale.y);
	Vec2 BtnBtm = Vec2(BtnPos.x + BtnScale.x, BtnPos.y - BtnScale.y);
	if (BtnLtp.x < CursorPos.x && CursorPos.x < BtnBtm.x
		&& BtnBtm.y < CursorPos.y && CursorPos.y < BtnLtp.y)
	{
		if(!m_bMouseOn)
		{
			m_pPlayButton->Animator2D()->GetCurAnim()->Stop(1);
			m_bMouseOn = true;
		}
		else
		{
			if (KEY_RELEASE(KEY::LBTN))
			{
				CLevel* pLoadedLevel = LoadLevel(L"Level\\TestLevel.lv");

				CCollisionMgr::GetInst()->LayerCheck((int)LAYER::PLAYER, (int)LAYER::MONSTER);
				CCollisionMgr::GetInst()->LayerCheck((int)LAYER::PLAYER, (int)LAYER::WALL);
				CCollisionMgr::GetInst()->LayerCheck((int)LAYER::MONSTER, (int)LAYER::WALL);
				CCollisionMgr::GetInst()->LayerCheck((int)LAYER::MONSTERPROJECTILE, (int)LAYER::WALL);
				CCollisionMgr::GetInst()->LayerCheck((int)LAYER::PLAYERPROJECTILE, (int)LAYER::WALL);
				CCollisionMgr::GetInst()->LayerCheck((int)LAYER::PLAYERPROJECTILE, (int)LAYER::MONSTER);
				CCollisionMgr::GetInst()->LayerCheck((int)LAYER::MONSTERPROJECTILE, (int)LAYER::PLAYER);
				CCollisionMgr::GetInst()->LayerCheck((int)LAYER::PLAYERPROJECTILE, (int)LAYER::ITEM);
				CCollisionMgr::GetInst()->LayerCheck((int)LAYER::MONSTERPROJECTILE, (int)LAYER::ITEM);
				CCollisionMgr::GetInst()->LayerCheck((int)LAYER::MONSTER, (int)LAYER::ITEM);
				CCollisionMgr::GetInst()->LayerCheck((int)LAYER::PLAYER, (int)LAYER::ITEM);
				CCollisionMgr::GetInst()->LayerCheck((int)LAYER::PLAYER, (int)LAYER::FALLAREA);
				CCollisionMgr::GetInst()->LayerCheck((int)LAYER::PLAYER, (int)LAYER::SPAWNBOX);
				CCollisionMgr::GetInst()->LayerCheck((int)LAYER::PLAYER, (int)LAYER::BOMB);
				CCollisionMgr::GetInst()->LayerCheck((int)LAYER::MONSTER, (int)LAYER::BOMB);
				CCollisionMgr::GetInst()->LayerCheck((int)LAYER::BOMB, (int)LAYER::ITEM);
				CCollisionMgr::GetInst()->LayerCheck((int)LAYER::BLANK, (int)LAYER::MONSTERPROJECTILE);
				CResMgr::GetInst()->FindRes<CSound>(L"MenuBGM")->Stop();
				CResMgr::GetInst()->FindRes<CSound>(L"BGM_Game")->Play(0, 0.1f, false);

				tEvent evn = {};
				evn.Type = EVENT_TYPE::LEVEL_CHANGE;
				evn.wParam = (DWORD_PTR)pLoadedLevel;

				CEventMgr::GetInst()->AddEvent(evn);
			}
		}
	}
	else
	{
		if (m_bMouseOn)
		{
			m_pPlayButton->Animator2D()->GetCurAnim()->Stop(0);
			CResMgr::GetInst()->FindRes<CSound>(L"MenuOn")->Play(1, 1.f, true);
			m_bMouseOn = false;
		}
	}

}

CLevel* CPlayButtonUIScript::LoadLevel(const wstring& _LevelPath)
{
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += _LevelPath;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strPath.c_str(), L"rb");

	if (nullptr == pFile)
		return nullptr;

	// 레벨 이름 불러오기
	CLevel* NewLevel = new CLevel;

	wstring strLevelName;
	LoadWString(strLevelName, pFile);
	NewLevel->SetName(strLevelName);

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// 레이어 이름 및 objCount를 읽어와 LoadGameObject
		CLayer* pLayer = NewLevel->GetLayer(i);

		wstring LayerName;
		LoadWString(LayerName, pFile);
		pLayer->SetName(LayerName);

		size_t objCount = 0;
		fread(&objCount, sizeof(size_t), 1, pFile);

		for (size_t j = 0; j < objCount; ++j)
		{
			CGameObject* pNewObj = LoadGameObject(pFile);
			NewLevel->AddGameObject(pNewObj, i, false);
		}
	}

	fclose(pFile);

	NewLevel->ChangeState(LEVEL_STATE::STOP);

	return NewLevel;
}

CGameObject* CPlayButtonUIScript::LoadGameObject(FILE* _File)
{
	CGameObject* pObject = new CGameObject;

	// 이름
	wstring Name;
	LoadWString(Name, _File);
	pObject->SetName(Name);

	// 컴포넌트
	while (true)
	{
		UINT ComponentType = 0;
		fread(&ComponentType, sizeof(UINT), 1, _File);

		if ((UINT)COMPONENT_TYPE::END == ComponentType)
			break;

		CComponent* Component = nullptr;

		switch ((COMPONENT_TYPE)ComponentType)
		{
		case COMPONENT_TYPE::TRANSFORM:
			Component = new CTransform();
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			Component = new CCollider2D();
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			//Component = new Collider3D();
			break;
		case COMPONENT_TYPE::ANIMATOR2D:
			Component = new CAnimator2D();
			break;
		case COMPONENT_TYPE::ANIMATOR3D:
			//Component = new CAnimator3D();
			break;
		case COMPONENT_TYPE::RIGIDBODY:
			Component = new CRigidbody();
			break;
		case COMPONENT_TYPE::LIGHT2D:
			Component = new CLight2D();
			break;
		case COMPONENT_TYPE::LIGHT3D:
			//Component = new CLight3D();
			break;
		case COMPONENT_TYPE::CAMERA:
			Component = new CCamera();
			break;
		case COMPONENT_TYPE::MESHRENDER:
			Component = new CMeshRender();
			break;
		case COMPONENT_TYPE::PARTICLESYSTEM:
			Component = new CParticleSystem(PARTICLE::END);
			break;
		case COMPONENT_TYPE::TILEMAP:
			Component = new CTileMap();
			break;
		case COMPONENT_TYPE::LANDSCAPE:
			//Component = new CLandScape();
			break;
		case COMPONENT_TYPE::DECAL:
			//Component = new CDecal();
			break;
		}
		Component->LoadFromLevelFile(_File);
		pObject->AddComponent(Component);
	}

	// 스크립트
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring ScriptName;
		LoadWString(ScriptName, _File);
		CScript* pScript = CScriptMgr::GetScript(ScriptName);
		pObject->AddComponent(pScript);
		pScript->LoadFromLevelFile(_File);
	}

	// 자식 obj
	size_t ChildCount = 0;
	fread(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		CGameObject* ChildObject = LoadGameObject(_File);
		pObject->AddChild(ChildObject);
	}

	return pObject;
}
