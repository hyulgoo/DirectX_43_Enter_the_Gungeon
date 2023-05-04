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

		// ��� UI ��Ŀ�� ����
		ImGui::SetWindowFocus(nullptr);
	}
}

int ListUI::render_update()
{
	// �ֱ� UI�� �۾����� ����� �˾Ƴ���
	ImVec2 ListUIContentSize = ImGui::GetContentRegionAvail();

	// List�ڽ� �̸� �� ����� �����Ͽ� ����
	if (ImGui::BeginListBox("##List", ListUIContentSize))
	{
		// ����Ʈ ���� �����͸� ���鼭 ���õ� ���� ������ ���̶���Ʈ ó��
		for (int i = 0; i < m_vecStrData.size(); i++)
		{
			const bool is_selected = (m_iSelectedIdx == i);

			if (ImGui::Selectable(m_vecStrData[i].c_str(), is_selected))
			{
				m_iSelectedIdx = i;
			}

			// Ư�� UI(������) ������ ����Ŭ���� �߻��ϸ�
			if (ImGui::IsItemHovered() &&
				ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				SetActive(false);
				ImGui::SetWindowFocus(nullptr);

				// Select Delegate�� ��ϵǾ� �ִٸ� ȣ���Ѵ�.
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
	// �ʱ�ȭ, �̸�, ������ ����, Ȱ��ȭ, 
	Clear();

	SetName(_strName);

	SetSize(_vSize.x, _vSize.y);

	SetActive(true);

	HWND hWnd = ImGuiMgr::GetInst()->GetMainHwnd();

	RECT rt = {};
	GetWindowRect(hWnd, &rt);

	// ����͸� �����Ͽ� Ŭ���̾�Ʈ �߾����� ��ǥ�� ����.
	float CenterX = (float)((rt.left + rt.right) / 2);
	float CenterY = (float)((rt.bottom + rt.top) / 2);

	// UI�� �ߴ� ��ġ�� �߾����� ����
	SetPopupPos(ImVec2(CenterX - _vSize.x / 2, CenterY - _vSize.y / 2.f));
}
