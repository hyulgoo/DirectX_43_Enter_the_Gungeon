#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine/CMeshRender.h>
#include <Engine/CResMgr.h>

#include "ImGuiMgr.h"
#include "ListUI.h"
#include <Engine/ptr.h>

MeshRenderUI::MeshRenderUI()
	: ComponentUI("##MeshRender", COMPONENT_TYPE::MESHRENDER)
{
	SetName("MeshRender");
}

MeshRenderUI::~MeshRenderUI()
{
}

int MeshRenderUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	char szBuff[50] = {};

	Ptr<CMesh> pMesh = GetTarget()->MeshRender()->GetMesh();
	Ptr<CMaterial> pMtrl = GetTarget()->MeshRender()->GetMaterial();

	ImGui::Text("Mesh    ");
	ImGui::SameLine();
	GetResKey(pMesh.Get(), szBuff, 50);
	// Mesh 이름은 함수로 바꿔선 안되기 때문에 읽기 전용으로 설정 
	ImGui::InputText("##MeshName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("##MeshSelectBtn", ImVec2(18.f, 18.f)))
	{
		// MeshList를 출력할 UI 생성과 선택됐을 때 호출될 Delegate 등록
		const map<wstring, Ptr<CRes>>& mapMesh = CResMgr::GetInst()->GetResources(RES_TYPE::MESH);

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("Mesh List", ImVec2(300.f, 500.f));
		for (const auto& pair : mapMesh)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}

		// 항목 선택 시 호출될 델리게이트 등록
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&MeshRenderUI::SelectMesh);
	}




	ImGui::Text("Material");
	ImGui::SameLine();
	GetResKey(pMtrl.Get(), szBuff, 50);
	// Mesh 이름은 함수로 바꿔선 안되기 때문에 읽기 전용으로 설정 
	ImGui::InputText("##MtrlName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("##MtrlSelectBtn", ImVec2(18.f, 18.f)))
	{
		const map<wstring, Ptr<CRes>>& mapMtrl = CResMgr::GetInst()->GetResources(RES_TYPE::MATERIAL);

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->Reset("Material List", ImVec2(300.f, 500.f));
		for (const auto& pair : mapMtrl)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}
		// 항목 선택 시 호출될 델리게이트 등록
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&MeshRenderUI::SelectMaterial);
	}

	return TRUE;
}

// Mesh, Material이 선택됐을 때 호출될 함수
void MeshRenderUI::SelectMesh(DWORD_PTR _Key)
{
	// _key로 선택된 Mesh를 찾아내 Target의 Mesh로 설정함
	string strKey = (char*)_Key;
	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->MeshRender()->SetMesh(pMesh);
}

void MeshRenderUI::SelectMaterial(DWORD_PTR _Key)
{
	// _key로 선택된 Mtrl를 찾아내 Target의 Mtrl로 설정함
	string strKey = (char*)_Key;
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->MeshRender()->SetMaterial(pMtrl);
}
