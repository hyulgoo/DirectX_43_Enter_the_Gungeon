#include "pch.h"
#include "CEndingLevelScript.h"

CEndingLevelScript::CEndingLevelScript()
    : CScript((UINT)ENDINGLEVELSCRIPT)
    , m_pEngineScreen(nullptr)
{
}

CEndingLevelScript::~CEndingLevelScript()
{
}

void CEndingLevelScript::begin()
{
    if (nullptr == m_pEngineScreen)
    {
        m_pEngineScreen = CResMgr::GetInst()->FindRes<CPrefab>(L"Ending")->Instantiate();
        m_pEngineScreen->Animator2D()->Play(L"Ending", false);
        m_pEngineScreen->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
        m_pEngineScreen->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
        m_pEngineScreen->MeshRender()->GetDynamicMaterial();
        SpawnGameObject(m_pEngineScreen, Vec3(0.f, 0.f, 1.f), (int)LAYER::UI);
        CResMgr::GetInst()->FindRes<CSound>(L"BaseStageBGM")->Play(1, 0.5f, false);
    }
}

void CEndingLevelScript::tick()
{
    if (LEVEL_STATE::STOP == CLevelMgr::GetInst()->GetCurLevel()->GetState())
        CLevelMgr::GetInst()->GetCurLevel()->ChangeState(LEVEL_STATE::PLAY);
}
