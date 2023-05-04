#include "pch.h"
#include "PrefabUI.h"
#include <Engine/CPrefab.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CPathMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CRes.h>
#include <Engine/CResMgr.h>
#include "ImGuiMgr.h"
#include "InspectorUI.h"

PrefabUI::PrefabUI()
    : ResUI(RES_TYPE::PREFAB)
    , m_iLayerNum(0)
{
    SetName("Prefab");
}

PrefabUI::~PrefabUI()
{

}

int PrefabUI::render_update()
{
    ResUI::render_update();

    Ptr<CPrefab> TargetPrefab = (CPrefab*)GetTargetRes().Get();
    wstring Name = TargetPrefab->m_ProtoObj->GetName();
    string TargetName = string(Name.begin(), Name.end());

    if (ImGui::Button("Save", ImVec2(60.f, 20.f)))
    {
        wstring strName = L"prefab\\";
        strName += TargetPrefab->m_ProtoObj->GetName();
        strName += L".pref";
        TargetPrefab->Save(strName);
    }
    ImGui::NewLine();
    InspectorUI* inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
    inspector->GetSpawnLayerNum();
    ImGui::SetNextItemWidth(50.f);
    ImGui::InputInt("SpawnLayerNum", &m_iLayerNum, ImGuiInputTextFlags_None);
    ImGui::SameLine();
    if (ImGui::Button("SetNum", ImVec2(60.f, 20.f)))
    {
        inspector->SetSpawnLayerNum(m_iLayerNum);
    }


    ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), TargetName.c_str());


    ShowOwnComponent(TargetPrefab.Get()->m_ProtoObj);

    vector<CGameObject*> vecChild = TargetPrefab->m_ProtoObj->GetChild();
    for (size_t i = 0; i < vecChild.size(); ++i)
    {
        wstring Name = vecChild[i]->GetName();
        string TargetName = string(Name.begin(), Name.end());
        ImGui::TextColored(ImVec4(0.f,1.f,1.f,1.f), TargetName.c_str());
        ShowOwnComponent(vecChild[i]);
    }
    return 0;
}
#include "CLevelSaveLoad.h"
int PrefabUI::SavePrefab(const wstring& _PrefabPath)
{
    wstring strPath = CPathMgr::GetInst()->GetContentPath();
    strPath += _PrefabPath;

    FILE* pFile = nullptr;

    _wfopen_s(&pFile, strPath.c_str(), L"wb");

    if (nullptr == pFile)
        return E_FAIL;

    Ptr<CPrefab> TargetPrefab = (CPrefab*)GetTargetRes().Get();
    
    SaveWString(GetTargetRes()->GetKey(), pFile);
    CLevelSaveLoad::SaveGameObject(TargetPrefab->m_ProtoObj, pFile);

    fclose(pFile);

    return S_OK;
}

void PrefabUI::ShowOwnComponent(CGameObject* _Obj)
{
    for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
    {
        if (nullptr != _Obj->GetComponent((COMPONENT_TYPE)i))
            ImGui::Text(ToString((COMPONENT_TYPE)i));
    }
    ImGui::NewLine();
}
//
//CPrefab* PrefabUI::LoadPrefab(const wstring& _PrefabPath)
//{
//    wstring strPath = CPathMgr::GetInst()->GetContentPath();
//    strPath += _PrefabPath;
//
//    FILE* pFile = nullptr;
//
//    _wfopen_s(&pFile, strPath.c_str(), L"rb");
//
//    if (nullptr == pFile)
//        return nullptr;
//
//    wstring PrefabName;
//    LoadWString(PrefabName, pFile);
//    
//    CPrefab* LoadPrefab = new CPrefab;
//    LoadPrefab->SetName(PrefabName);
//
//    CGameObject* LoadProtoObj = CLevelSaveLoad::LoadGameObject(pFile);
//    LoadPrefab->m_ProtoObj = LoadProtoObj;
//}
