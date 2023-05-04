#include "pch.h"
#include "MenuUI.h"

#include "CLevelSaveLoad.h"
#include "OutlinerUI.h"
#include "InspectorUI.h"
#include "ImGuiMgr.h"

#include <Script/CScriptMgr.h>

#include <Engine/CGameObject.h>
#include <Engine/CEventMgr.h>
#include <Engine/components.h>
#include <Engine/CScript.h>
#include <Engine/CRenderMgr.h>

MenuUI::MenuUI()
	: UI("##Menu")
	, m_pCurColliderObj(nullptr)
	, m_bWall(false)
{
	SetName("Menu");
}

MenuUI::~MenuUI()
{
}

void MenuUI::finaltick()
{
	if (!IsActive())
		return;

	//if (KEY_TAP(KEY::LBTN) && m_pCurColliderObj != nullptr && !m_bWall)
	//{
	//	m_WallLtp = CRenderMgr::GetInst()->GetRay();
	//	m_bWall = true;
	//}
	//
	//if (m_bWall)
	//{
	//	if(KEY_PRESSED(KEY::LBTN))
	//		m_WallBtm = CRenderMgr::GetInst()->GetRay();
	//	if (KEY_RELEASE(KEY::LBTN))
	//	{
	//		m_WallBtm = CRenderMgr::GetInst()->GetRay();
	//		m_bWall = false;
	//	}
	//	Vec3 vScale = Vec3(abs(m_WallBtm.x - m_WallLtp.x), abs(m_WallLtp.y - m_WallBtm.y), 1.f);
	//	Vec3 vPos = Vec3(m_WallLtp.x + vScale.x / 2, m_WallLtp.y - vScale.y / 2, 1.f);
	//	m_pCurColliderObj->Transform()->SetRelativeScale(vScale);
	//	m_pCurColliderObj->Transform()->SetRelativePos(vPos);
	//	if(!m_bWall)
	//		m_pCurColliderObj = nullptr;
	//}
	//if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::LBTN))
	//	CreateWall();

	render_update();
}

int MenuUI::render_update()
{
	if (ImGui::BeginMainMenuBar())
	{
		// 레벨 세이브 로드
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save Level"))
			{
				CLevelSaveLoad::SaveLevel(L"Level\\TestLevel.lv", CLevelMgr::GetInst()->GetCurLevel());
			}

			if (ImGui::MenuItem("Save Ending"))
			{
				CLevelSaveLoad::SaveLevel(L"Level\\EndingLevel.lv", CLevelMgr::GetInst()->GetCurLevel());
			}

			if (ImGui::MenuItem("Load Level"))
			{
				CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(L"Level\\TestLevel.lv");

				tEvent evn = {};
				evn.Type = EVENT_TYPE::LEVEL_CHANGE;
				evn.wParam = (DWORD_PTR)pLoadedLevel;

				CEventMgr::GetInst()->AddEvent(evn);
			}

			ImGui::EndMenu();
		}
		// 오브젝트 생성, 컴포넌트 추가, 스크립트 추가
		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Create Empty Object"))
			{
				CreateEmptyObject();
			}
			ImGui::Separator();

			if (ImGui::MenuItem("Create Wall"))
			{
				CreateWall();
			}
			ImGui::Separator();
			if (ImGui::BeginMenu("Add Component"))
			{
				for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
				{
					if (ImGui::MenuItem(ToString((COMPONENT_TYPE)i)))
					{
						AddComponent(COMPONENT_TYPE(i));
					}
				}
				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Add Script"))
			{
				// 전체 스크립트 이름 목록을 가져온다.
				vector<wstring> vecScripts;
				CScriptMgr::GetScriptInfo(vecScripts);

				// 스크립트 이름 목록 숫자만큼 메뉴아이템을 추가한다.
				for (size_t i = 0; i < vecScripts.size(); ++i)
				{
					string strScriptName = string(vecScripts[i].begin(), vecScripts[i].end());
					if (ImGui::MenuItem(strScriptName.c_str()))
					{
						// 스크립트가 선택됐다면 해당 이름을 가지고 있는 실제 스크립트를 찾아서 넣어준다.
						AddScript(vecScripts[i]);
					}
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Level"))
		{
			CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
			bool PlayEnable = true;
			bool PauseEnable = true;
			bool StopEnable = true;

			if (pCurLevel->GetState() == LEVEL_STATE::PLAY)
			{
				PlayEnable = false;
				PauseEnable = true;
				StopEnable = true;
			}
			else if (pCurLevel->GetState() == LEVEL_STATE::PAUSE)
			{
				PlayEnable = true;
				PauseEnable = false;
				StopEnable = true;
			}
			else if (pCurLevel->GetState() == LEVEL_STATE::STOP)
			{
				PlayEnable = true;
				PauseEnable = false;
				StopEnable = false;

			}

			if (ImGui::MenuItem("Play", nullptr, nullptr, PlayEnable))
			{
				CLevelSaveLoad::SaveLevel(L"Level\\Temp.lv", pCurLevel);
				pCurLevel->ChangeState(LEVEL_STATE::PLAY);
			}
			else if (ImGui::MenuItem("Pause", nullptr, nullptr, PauseEnable))
			{
				pCurLevel->ChangeState(LEVEL_STATE::PAUSE);
			}
			else if (ImGui::MenuItem("Stop", nullptr, nullptr, StopEnable))
			{
				pCurLevel->ChangeState(LEVEL_STATE::STOP);
				CLevel* pNewLevel = CLevelSaveLoad::LoadLevel(L"Level\\Temp.lv");

				// 불러온 임시레벨로 레벨 체인지
				tEvent evn = {};
				evn.Type = EVENT_TYPE::LEVEL_CHANGE;
				evn.wParam = DWORD_PTR(pNewLevel);
				CEventMgr::GetInst()->AddEvent(evn);

				// InspectorUI가 참조하고 있던 삭제된 레벨의 obj를 초기화
				InspectorUI* Inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
				Inspector->SetTargetObject(nullptr);						
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	return 0;
}

void MenuUI::CreateEmptyObject()
{
	// 새로운 obj를 추가해서 기본 레이어에 넣어줌
	CGameObject* pNewObject = new CGameObject;
	pNewObject->AddComponent(new CTransform);
	pNewObject->SetName(L"New Object");
	SpawnGameObject(pNewObject, Vec3(0.f, 0.f, 0.f), L"Default");

	OutlinerUI* outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");

	// 스폰obj로 outliner가 리셋됐기 때문에 추가될 obj를 정보를 가진 노드를 리셋 후 선택상태로 두게 함.
	outliner->SetSelectedNodeData(DWORD_PTR(pNewObject));
}

void MenuUI::CreateWall()
{
	// 새로운 obj를 추가해서 기본 레이어에 넣어줌
	CGameObject* pNewObject = new CGameObject;
	pNewObject->AddComponent(new CTransform);
	pNewObject->AddComponent(new CCollider2D);
	pNewObject->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
	pNewObject->SetName(L"Wall");
	SpawnGameObject(pNewObject, Vec3(0.f, 0.f, 0.f), (int)LAYER::WALL);

	m_pCurColliderObj = pNewObject;

	OutlinerUI* outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");

	// 스폰obj로 outliner가 리셋됐기 때문에 추가될 obj를 정보를 가진 노드를 리셋 후 선택상태로 두게 함.
	outliner->SetSelectedNodeData(DWORD_PTR(pNewObject));
}

void MenuUI::AddComponent(COMPONENT_TYPE _type)
{
	// Outliner와 Inspector를 가져옴
	OutlinerUI* outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");
	InspectorUI* inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");

	// 선택된 obj를 가져와 타입에 맞는 컴포넌트를 추가해줌.
	CGameObject* pSelectedObject = outliner->GetSelectedObject();

	// 선택 obj가 없다면 return
	if (nullptr == pSelectedObject)
		return;

	// 타입에 맞게 컴포넌트 추가
	switch (_type)
	{
	case COMPONENT_TYPE::TRANSFORM:
		pSelectedObject->AddComponent(new CTransform);
		break;
	case COMPONENT_TYPE::COLLIDER2D:
		pSelectedObject->AddComponent(new CCollider2D);
		break;
	case COMPONENT_TYPE::COLLIDER3D:
		//pSelectedObject->AddComponent(new CCollider3D);
		break;
	case COMPONENT_TYPE::ANIMATOR2D:
		pSelectedObject->AddComponent(new CAnimator2D);
		break;
	case COMPONENT_TYPE::ANIMATOR3D:
		//pSelectedObject->AddComponent(new CAnimator3D);
		break;
	case COMPONENT_TYPE::RIGIDBODY:
		pSelectedObject->AddComponent(new CRigidbody);
		break;
	case COMPONENT_TYPE::LIGHT2D:
		pSelectedObject->AddComponent(new CLight2D);
		break;
	case COMPONENT_TYPE::LIGHT3D:
		//pSelectedObject->AddComponent(new CLight3D);
		break;
	case COMPONENT_TYPE::CAMERA:
		pSelectedObject->AddComponent(new CCamera);
		break;
	case COMPONENT_TYPE::MESHRENDER:
		pSelectedObject->AddComponent(new CMeshRender);
		break;
	case COMPONENT_TYPE::PARTICLESYSTEM:
		pSelectedObject->AddComponent(new CParticleSystem(PARTICLE::END));
		break;
	case COMPONENT_TYPE::TILEMAP:
		pSelectedObject->AddComponent(new CTileMap);
		break;
	case COMPONENT_TYPE::LANDSCAPE:
		//pSelectedObject->AddComponent(new LANDSCAPE);
		break;
	case COMPONENT_TYPE::DECAL:
		//pSelectedObject->AddComponent(new DECAL);
		break;
	}
	// 컴포넌트 추가한 obj를 타겟으로 다시 지정하여 컴포넌트 정보 갱신
	inspector->SetTargetObject(pSelectedObject);
}

void MenuUI::AddScript(const wstring& _strScriptName)
{
	// 아웃라이너를 가져와 현재 선택된 obj를 가져온다.
	OutlinerUI* outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");
	InspectorUI* inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
	CGameObject* pSelectedObject = outliner->GetSelectedObject();

	// 현재 선택된 obj가 없다면 return
	if (nullptr == pSelectedObject)
		return;

	// 선택된 스크립트의 이름을 가지고 실제 스크립트를 찾아 선택된 obj에 넣어준다.
	CScript* pScript = CScriptMgr::GetScript(_strScriptName);

	pSelectedObject->AddComponent(pScript);

	// 스크립트를 추가한 후 다시 InspectorUI에 알려 추가된 스크립트를 갱신해줘야 함.
	inspector->SetTargetObject(pSelectedObject);
}
