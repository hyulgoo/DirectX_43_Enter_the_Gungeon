#include "pch.h"
#include "InspectorUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CResMgr.h>
#include <Engine\CKeyMgr.h>

#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "Collider2DUI.h"
#include "CameraUI.h"
#include "Animator2DUI.h"
#include "TileMapUI.h"
#include "Light2DUI.h"
#include "ParticleSystemUI.h"
#include "RigidbodyUI.h"

#include "MeshDataUI.h"
#include "TextureUI.h"
#include "MeshUI.h"
#include "SoundUI.h"
#include "PrefabUI.h"
#include "GraphicsShaderUI.h"
#include "ComputeShaderUI.h"
#include "MaterialUI.h"
#include "ScriptUI.h"

#include "OutlinerUI.h"
#include "ContentUI.h"

InspectorUI::InspectorUI()
	: UI("##Inspector")
	, m_pTargetObj(nullptr)
	, m_arrComUI{}
	, m_arrResUI{}
	, m_NewName{}
	, m_iLayerNum(0)
{
	SetName("Inspector");

	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetSize(0.f, 120.f);
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = new MeshRenderUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->SetSize(0.f, 80.f);
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]);

	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = new CameraUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]);

	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new Animator2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]->SetSize(0.f, 80.f);
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]); 

	m_arrComUI[(UINT)COMPONENT_TYPE::RIGIDBODY] = new RigidbodyUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::RIGIDBODY]->SetSize(0.f, 50.f);
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::RIGIDBODY]);

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetSize(0.f, 150.f);
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP] = new TileMapUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]->SetSize(0.f, 800.f);
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]);

	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM] = new ParticleSystemUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]->SetSize(0.f, 800.f);
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]);
	
	// ResUI
	m_arrResUI[(UINT)RES_TYPE::MESHDATA] = new MeshDataUI;
	m_arrResUI[(UINT)RES_TYPE::MESHDATA]->SetSize(0.f, 0.f);
	AddChildUI(m_arrResUI[(UINT)RES_TYPE::MESHDATA]);

	m_arrResUI[(UINT)RES_TYPE::MESH] = new MeshUI;
	m_arrResUI[(UINT)RES_TYPE::MESH]->SetSize(0.f, 0.f);
	AddChildUI(m_arrResUI[(UINT)RES_TYPE::MESH]);

	m_arrResUI[(UINT)RES_TYPE::TEXTURE] = new TextureUI;
	m_arrResUI[(UINT)RES_TYPE::TEXTURE]->SetSize(0.f, 0.f);
	AddChildUI(m_arrResUI[(UINT)RES_TYPE::TEXTURE]);

	m_arrResUI[(UINT)RES_TYPE::GRAPHICS_SHADER] = new GraphicsShaderUI;
	m_arrResUI[(UINT)RES_TYPE::GRAPHICS_SHADER]->SetSize(0.f, 0.f);
	AddChildUI(m_arrResUI[(UINT)RES_TYPE::GRAPHICS_SHADER]);

	m_arrResUI[(UINT)RES_TYPE::COMPUTE_SHADER] = new ComputeShaderUI;
	m_arrResUI[(UINT)RES_TYPE::COMPUTE_SHADER]->SetSize(0.f, 0.f);
	AddChildUI(m_arrResUI[(UINT)RES_TYPE::COMPUTE_SHADER]);

	m_arrResUI[(UINT)RES_TYPE::PREFAB] = new PrefabUI;
	m_arrResUI[(UINT)RES_TYPE::PREFAB]->SetSize(0.f, 0.f);
	AddChildUI(m_arrResUI[(UINT)RES_TYPE::PREFAB]);

	m_arrResUI[(UINT)RES_TYPE::MATERIAL] = new MaterialUI;
	m_arrResUI[(UINT)RES_TYPE::MATERIAL]->SetSize(0.f, 0.f);
	AddChildUI(m_arrResUI[(UINT)RES_TYPE::MATERIAL]);

	m_arrResUI[(UINT)RES_TYPE::SOUND] = new SoundUI;
	m_arrResUI[(UINT)RES_TYPE::SOUND]->SetSize(0.f, 0.f);
	AddChildUI(m_arrResUI[(UINT)RES_TYPE::SOUND]);
}

InspectorUI::~InspectorUI()
{

}

void InspectorUI::init()
{
	SetTargetObject(nullptr);
}

void InspectorUI::tick()
{

}

int InspectorUI::render_update()
{
	SetTargetName(); 
	AddTargetToPrefab();
	SpawnPrefab();

	return TRUE;
}

void InspectorUI::SetTargetObject(CGameObject* _Target)
{
	ClearTargetResource();

	// 타겟오브젝트 정보 노출
	m_pTargetObj = _Target;
	if(nullptr != _Target)
	{
		wstring m_strTargetName = m_pTargetObj->GetName();
		int iLen = WideCharToMultiByte(CP_ACP, 0, m_strTargetName.c_str(), -1, NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, m_strTargetName.c_str(), -1, m_NewName, iLen, 0, 0);
	}

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;

		m_arrComUI[i]->SetTarget(m_pTargetObj);
	}

	if (nullptr == m_pTargetObj)
	{
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->SetActive(false);
		}
		return;
	}

	// 멤버로 갖는 vec보다 가져온 vec의 크기가 크다면 차이만큼 늘려줌.
	const vector<CScript*>& vecScript = m_pTargetObj->GetScripts();

	if (m_vecScriptUI.size() < vecScript.size())
	{
		size_t iDiffer = vecScript.size() - m_vecScriptUI.size();
		for (size_t i = 0; i < iDiffer; ++i)
		{
			ScriptUI* UI = new ScriptUI;

			m_vecScriptUI.push_back(UI);
			AddChildUI(UI);
			UI->SetActive(true);
		}
	}

	// 멤버로 갖는 vec가 Target에서 가져온 vec보다 크다면 차이만큼 fasle로 돌려주고
	// 나머지는 Script 세팅하고 true로 돌려줌.
	for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
	{
		if (vecScript.size() <= i)
		{
			m_vecScriptUI[i]->SetActive(false);
			continue;
		}

		m_vecScriptUI[i]->SetTarget(m_pTargetObj);
		m_vecScriptUI[i]->SetScript(vecScript[i]);
		m_vecScriptUI[i]->SetActive(true);
	}
}

void InspectorUI::SetTargetResource(Ptr<CRes> _Res)
{
	ClearTargetObject();

	for (UINT i = 0; i < UINT(RES_TYPE::END); ++i)
	{
		m_arrResUI[i]->SetActive(false);
	}

	m_pTargetRes = _Res;

	if (nullptr == m_pTargetRes)
		return;

	RES_TYPE type = _Res->GetType();

	m_arrResUI[(UINT)type]->SetActive(true);
	m_arrResUI[(UINT)type]->SetTargetRes(_Res);
}

void InspectorUI::ClearTargetObject()
{
	// 타겟오브젝트 정보 노출
	m_pTargetObj = nullptr;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;

		m_arrComUI[i]->SetTarget(nullptr);
	}
}

void InspectorUI::ClearTargetResource()
{
	m_pTargetRes = nullptr;

	for (UINT i = 0; i < UINT(RES_TYPE::END); ++i)
	{
		if (nullptr != m_arrResUI[i])
		{
			m_arrResUI[i]->SetTargetRes(nullptr);
			m_arrResUI[i]->SetActive(false);
		}
	}
}

void InspectorUI::SetTargetName()
{
	if (nullptr == m_pTargetObj)
		return;
	ImGui::InputText("##Name", m_NewName, 50);
	ImGui::SameLine();
	if (ImGui::Button("SetName", ImVec2(80.f, 20.f)))
	{
		wstring strNewName;
		int iLen = MultiByteToWideChar(CP_ACP, 0, m_NewName, -1, NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, m_NewName, strlen(m_NewName) + 1, (wchar_t*)strNewName.c_str(), iLen);
		m_pTargetObj->SetName(strNewName.c_str());
		OutlinerUI* outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");
		outliner->ResetOutliner();
	}	
}

void InspectorUI::AddTargetToPrefab()
{
	if (nullptr == m_pTargetObj)
		return;
	if(ImGui::Button("SavePrefab", ImVec2(100.f, 20.f)))
	{
		CGameObject* PrefabObj = m_pTargetObj->Clone();
		Ptr<CPrefab> Prefab = new CPrefab;
		Prefab->RegisterProtoObject(PrefabObj);

		CResMgr::GetInst()->AddRes<CPrefab>(m_pTargetObj->GetName(), Prefab);
		ContentUI* content = (ContentUI*)ImGuiMgr::GetInst()->FindUI("##Content");
		content->ResetContent();
	}

	if (ImGui::Button("DELETE", ImVec2(100.f, 20.f)))
	{
		m_pTargetObj->SetLifeSpan(0.f);
		SetTargetObject(nullptr);

	}
}
#include <Engine/CRenderMgr.h>
void InspectorUI::SpawnPrefab()
{
	if (m_pTargetRes != nullptr && RES_TYPE::PREFAB == m_pTargetRes->GetType())
	{
		if (KEY_PRESSED(KEY::LCTRL))
		{
			if(KEY_TAP(KEY::LBTN))
			{
				CPrefab* PrefabTarget = (CPrefab*)m_pTargetRes.Get();
				SpawnGameObject(PrefabTarget->Instantiate(), CRenderMgr::GetInst()->GetRay(), m_iLayerNum);
			}
		}
	}
}
