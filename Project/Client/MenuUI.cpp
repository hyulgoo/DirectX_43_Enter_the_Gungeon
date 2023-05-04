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
		// ���� ���̺� �ε�
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
		// ������Ʈ ����, ������Ʈ �߰�, ��ũ��Ʈ �߰�
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
				// ��ü ��ũ��Ʈ �̸� ����� �����´�.
				vector<wstring> vecScripts;
				CScriptMgr::GetScriptInfo(vecScripts);

				// ��ũ��Ʈ �̸� ��� ���ڸ�ŭ �޴��������� �߰��Ѵ�.
				for (size_t i = 0; i < vecScripts.size(); ++i)
				{
					string strScriptName = string(vecScripts[i].begin(), vecScripts[i].end());
					if (ImGui::MenuItem(strScriptName.c_str()))
					{
						// ��ũ��Ʈ�� ���õƴٸ� �ش� �̸��� ������ �ִ� ���� ��ũ��Ʈ�� ã�Ƽ� �־��ش�.
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

				// �ҷ��� �ӽ÷����� ���� ü����
				tEvent evn = {};
				evn.Type = EVENT_TYPE::LEVEL_CHANGE;
				evn.wParam = DWORD_PTR(pNewLevel);
				CEventMgr::GetInst()->AddEvent(evn);

				// InspectorUI�� �����ϰ� �ִ� ������ ������ obj�� �ʱ�ȭ
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
	// ���ο� obj�� �߰��ؼ� �⺻ ���̾ �־���
	CGameObject* pNewObject = new CGameObject;
	pNewObject->AddComponent(new CTransform);
	pNewObject->SetName(L"New Object");
	SpawnGameObject(pNewObject, Vec3(0.f, 0.f, 0.f), L"Default");

	OutlinerUI* outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");

	// ����obj�� outliner�� ���µƱ� ������ �߰��� obj�� ������ ���� ��带 ���� �� ���û��·� �ΰ� ��.
	outliner->SetSelectedNodeData(DWORD_PTR(pNewObject));
}

void MenuUI::CreateWall()
{
	// ���ο� obj�� �߰��ؼ� �⺻ ���̾ �־���
	CGameObject* pNewObject = new CGameObject;
	pNewObject->AddComponent(new CTransform);
	pNewObject->AddComponent(new CCollider2D);
	pNewObject->Collider2D()->SetOffsetScale(Vec2(1.f, 1.f));
	pNewObject->SetName(L"Wall");
	SpawnGameObject(pNewObject, Vec3(0.f, 0.f, 0.f), (int)LAYER::WALL);

	m_pCurColliderObj = pNewObject;

	OutlinerUI* outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");

	// ����obj�� outliner�� ���µƱ� ������ �߰��� obj�� ������ ���� ��带 ���� �� ���û��·� �ΰ� ��.
	outliner->SetSelectedNodeData(DWORD_PTR(pNewObject));
}

void MenuUI::AddComponent(COMPONENT_TYPE _type)
{
	// Outliner�� Inspector�� ������
	OutlinerUI* outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");
	InspectorUI* inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");

	// ���õ� obj�� ������ Ÿ�Կ� �´� ������Ʈ�� �߰�����.
	CGameObject* pSelectedObject = outliner->GetSelectedObject();

	// ���� obj�� ���ٸ� return
	if (nullptr == pSelectedObject)
		return;

	// Ÿ�Կ� �°� ������Ʈ �߰�
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
	// ������Ʈ �߰��� obj�� Ÿ������ �ٽ� �����Ͽ� ������Ʈ ���� ����
	inspector->SetTargetObject(pSelectedObject);
}

void MenuUI::AddScript(const wstring& _strScriptName)
{
	// �ƿ����̳ʸ� ������ ���� ���õ� obj�� �����´�.
	OutlinerUI* outliner = (OutlinerUI*)ImGuiMgr::GetInst()->FindUI("##Outliner");
	InspectorUI* inspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
	CGameObject* pSelectedObject = outliner->GetSelectedObject();

	// ���� ���õ� obj�� ���ٸ� return
	if (nullptr == pSelectedObject)
		return;

	// ���õ� ��ũ��Ʈ�� �̸��� ������ ���� ��ũ��Ʈ�� ã�� ���õ� obj�� �־��ش�.
	CScript* pScript = CScriptMgr::GetScript(_strScriptName);

	pSelectedObject->AddComponent(pScript);

	// ��ũ��Ʈ�� �߰��� �� �ٽ� InspectorUI�� �˷� �߰��� ��ũ��Ʈ�� ��������� ��.
	inspector->SetTargetObject(pSelectedObject);
}
