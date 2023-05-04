#include "pch.h"
#include "CBossDeath.h"
#include "CBossScript.h"
#include <Engine/CEventMgr.h>

CBossDeath::CBossDeath()
{
}

CBossDeath::~CBossDeath()
{
}

void CBossDeath::tick()
{
	if (KEY_TAP(KEY::O))
	{

		CLevel* pLoadedLevel = LoadLevel(L"Level\\EndingLevel.lv");

		tEvent evn = {};
		evn.Type = EVENT_TYPE::LEVEL_CHANGE;
		evn.wParam = (DWORD_PTR)pLoadedLevel;

		CEventMgr::GetInst()->AddEvent(evn);
	}
}

void CBossDeath::Enter()
{
	GetOwner()->DeleteComponent((UINT)COMPONENT_TYPE::COLLIDER2D);
	CResMgr::GetInst()->FindRes<CSound>(L"BGM_Boss")->Stop();
	CResMgr::GetInst()->FindRes<CSound>(L"Galmea_Charge")->Play(1);
	CResMgr::GetInst()->FindRes<CSound>(L"FinalShot")->Play(1, 1.f);
	PlayAnim();
	Vec3 CurPos = GetOwner()->Transform()->GetWorldPos();
	GetOwner()->Transform()->SetRelativePos(CurPos.x, CurPos.y, CurPos.z + 200.f);
	GetOwner()->GetScript<CBossScript>()->SetRotable(false);
}

void CBossDeath::Exit()
{
}

void CBossDeath::PlayAnim()
{
	GetOwner()->Animator2D()->Play(L"Boss_Death_Left", false);
}

CLevel* CBossDeath::LoadLevel(const wstring& _LevelPath)
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

	NewLevel->ChangeState(LEVEL_STATE::PLAY);

	return NewLevel;
}

CGameObject* CBossDeath::LoadGameObject(FILE* _File)
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
