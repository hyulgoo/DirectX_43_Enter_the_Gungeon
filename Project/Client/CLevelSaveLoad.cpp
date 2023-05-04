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
	// Level�� ���� ���°� �ƴ϶�� �������� ����.
	if(_Level->GetState() != LEVEL_STATE::STOP)
	return E_FAIL;

	// ���� ��� ����
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += _LevelPath;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strPath.c_str(), L"wb");

	// ������ ������ ���ߴٸ� return
	if (nullptr == pFile)
		return E_FAIL;

	// Level �̸� ����
	SaveWString(_Level->GetName(), pFile);

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// Layer �̸� ����
		CLayer* pLayer = _Level->GetLayer(i);

		SaveWString(pLayer->GetName(), pFile);

		// Layer�� Parentobj ����� ������
		const vector<CGameObject*>& vecParent = pLayer->GetParentObject();

		size_t objCount = vecParent.size();
		fwrite(&objCount, sizeof(size_t), 1, pFile);

		for (size_t i = 0; i < objCount; ++i)
		{
			// Parentobj��ŭ �ݺ����� ���� obj ����
			SaveGameObject(vecParent[i], pFile);
		}
	}
	fclose(pFile);

	return S_OK;
}

int CLevelSaveLoad::SaveGameObject(CGameObject* _Object, FILE* _File)
{
	// obj �̸�
	SaveWString(_Object->GetName(), _File);

	// ������Ʈ
	for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (i == (UINT)COMPONENT_TYPE::END)
		{
			// ������Ʈ ������ �����ٸ� (UINT)COMPONENT_TYPE::END�� ���
			fwrite(&i, sizeof(UINT), 1, _File);
			break;
		}

		CComponent* Com = _Object->GetComponent((COMPONENT_TYPE)i);
		// ������Ʈ�� ���ٸ� �Ѿ��
		if (nullptr == Com)
			continue;

		// ������Ʈ Ÿ�� ����
		fwrite(&i ,sizeof(UINT), 1, _File);

		// ������Ʈ �������� ���� ȣ��
		Com->SaveToLevelFile(_File);
	}

	// ��ũ��Ʈ
	const vector<CScript*>& vecScript = _Object->GetScripts();
	size_t ScriptCount = vecScript.size();
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecScript[i]);
		SaveWString(ScriptName, _File);
		vecScript[i]->SaveToLevelFile(_File);
	}

	// �ڽ� obj
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

	// ���� �̸� �ҷ�����
	CLevel* NewLevel = new CLevel;

	wstring strLevelName;
	LoadWString(strLevelName, pFile);
	NewLevel->SetName(strLevelName);

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// ���̾� �̸� �� objCount�� �о�� LoadGameObject
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

	// �̸�
	wstring Name;
	LoadWString(Name, _File);
	pObject->SetName(Name);

	// ������Ʈ
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

	// ��ũ��Ʈ
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

	// �ڽ� obj
	size_t ChildCount = 0;
	fread(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		CGameObject* ChildObject = LoadGameObject(_File);
		pObject->AddChild(ChildObject);
	}


	return pObject;
}
