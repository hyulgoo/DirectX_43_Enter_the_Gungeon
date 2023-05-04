#include "pch.h"
#include "ListUI.h"

#include <Engine/CKeyMgr.h>

ListUI::ListUI()
	: UI("##List")
	, m_vecStrData{}
	, m_iSelectedIdx(0)
	, m_SelectInst(nullptr)
	, m_SelectDelegate(nullptr)
{
}

ListUI::~ListUI()
{
}

void ListUI::finaltick()
{
	UI::finaltick();

	if (KEY_TAP(KEY::ESC))
	{
		SetActive(false);

		// 모든 UI 포커스 해제
		ImGui::SetWindowFocus(nullptr);
	}
}

int ListUI::render_update()
{
	// 최근 UI의 작업영역 사이즈를 알아낸다
	ImVec2 ListUIContentSize = ImGui::GetContentRegionAvail();

	// List박스 이름 및 사이즈를 지정하여 생성
	if (ImGui::BeginListBox("##List", ListUIContentSize))
	{
		// 리스트 안의 데이터를 돌면서 선택된 것이 있으면 하이라이트 처리
		for (int i = 0; i < m_vecStrData.size(); i++)
		{
			const bool is_selected = (m_iSelectedIdx == i);

			if (ImGui::Selectable(m_vecStrData[i].c_str(), is_selected))
			{
				m_iSelectedIdx = i;
			}

			// 특정 UI(아이템) 위에서 더블클릭이 발생하면
			if (ImGui::IsItemHovered() &&
				ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				SetActive(false);
				ImGui::SetWindowFocus(nullptr);

				// Select Delegate가 등록되어 있다면 호출한다.
				if (m_SelectInst && m_SelectDelegate)
				{
					(m_SelectInst->*m_SelectDelegate)((DWORD_PTR)m_vecStrData[i].c_str());
				}
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}

	return TRUE;
}

void ListUI::Reset(const string& _strName, ImVec2 _vSize)
{
	// 초기화, 이름, 사이즈 설정, 활성화, 
	Clear();

	SetName(_strName);

	SetSize(_vSize.x, _vSize.y);

	SetActive(true);

	HWND hWnd = ImGuiMgr::GetInst()->GetMainHwnd();

	RECT rt = {};
	GetWindowRect(hWnd, &rt);

	// 모니터를 포함하여 클라이언트 중앙으로 좌표를 맞춤.
	float CenterX = (float)((rt.left + rt.right) / 2);
	float CenterY = (float)((rt.bottom + rt.top) / 2);

	// UI가 뜨는 위치를 중앙으로 지정
	SetPopupPos(ImVec2(CenterX - _vSize.x / 2, CenterY - _vSize.y / 2.f));
}
