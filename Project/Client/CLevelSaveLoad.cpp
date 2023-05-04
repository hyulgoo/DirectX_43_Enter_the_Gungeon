#include "pch.h"
#include "CLevelSaveLoad.h"

#include <Engine/CPathMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Engine/CScript.h>
#include <Script/CScriptMgr.h>

int CLevelSaveLoad::SaveLevel(const wstring& _LevelPath, CLevel* _Level)
{
	// Level이 멈춘 상태가 아니라면 저장하지 않음.
	if(_Level->GetState() != LEVEL_STATE::STOP)
	return E_FAIL;

	// 레벨 경로 지정
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += _LevelPath;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strPath.c_str(), L"wb");

	// 파일을 만들지 못했다면 return
	if (nullptr == pFile)
		return E_FAIL;

	// Level 이름 저장
	SaveWString(_Level->GetName(), pFile);

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// Layer 이름 저장
		CLayer* pLayer = _Level->GetLayer(i);

		SaveWString(pLayer->GetName(), pFile);

		// Layer의 Parentobj 목록을 가져옴
		const vector<CGameObject*>& vecParent = pLayer->GetParentObject();

		size_t objCount = vecParent.size();
		fwrite(&objCount, sizeof(size_t), 1, pFile);

		for (size_t i = 0; i < objCount; ++i)
		{
			// Parentobj만큼 반복문을 돌며 obj 저장
			SaveGameObject(vecParent[i], pFile);
		}
	}
	fclose(pFile);

	return S_OK;
}

int CLevelSaveLoad::SaveGameObject(CGameObject* _Object, FILE* _File)
{
	// obj 이름
	SaveWString(_Object->GetName(), _File);

	// 컴포넌트
	for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (i == (UINT)COMPONENT_TYPE::END)
		{
			// 컴포넌트 저장이 끝났다면 (UINT)COMPONENT_TYPE::END만 기록
			fwrite(&i, sizeof(UINT), 1, _File);
			break;
		}

		CComponent* Com = _Object->GetComponent((COMPONENT_TYPE)i);
		// 컴포넌트가 없다면 넘어가기
		if (nullptr == Com)
			continue;

		// 컴포넌트 타입 저장
		fwrite(&i ,sizeof(UINT), 1, _File);

		// 컴포넌트 세부정보 저장 호출
		Com->SaveToLevelFile(_File);
	}

	// 스크립트
	const vector<CScript*>& vecScript = _Object->GetScripts();
	size_t ScriptCount = vecScript.size();
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecScript[i]);
		SaveWString(ScriptName, _File);
		vecScript[i]->SaveToLevelFile(_File);
	}

	// 자식 obj
	const vector<CGameObject*>& vecChild = _Object->GetChild();
	size_t ChildCount = vecChild.size();
	fwrite(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		SaveGameObject(vecChild[i], _File);
	}

	return 0;
}

CLevel* CLevelSaveLoad::LoadLevel(const wstring& _LevelPath)
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
			// SpawnGameObject(pNewObj, Vec3(0.f, 0.f, 0.f), i);
		}
	}

	fclose(pFile);

	NewLevel->ChangeState(LEVEL_STATE::STOP);

	return NewLevel;
}

CGameObject* CLevelSaveLoad::LoadGameObject(FILE* _File)
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
