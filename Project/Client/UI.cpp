#include "pch.h"
#include "UI.h"

UI::UI(const string& _ID)
	: m_strID(_ID)
	, m_ParentUI(nullptr)
	, m_Active(false)
	, m_Modal(false)
{
}

UI::~UI()
{
	Safe_Del_Vec(m_vecChildUI);
}
void UI::finaltick()
{
	if (!m_Active)
		return;

	string strFullName = m_strName + m_strID;

	// �ڽ��� �θ�UI���
	if (nullptr == m_ParentUI)
	{
		// ��޹���� �ƴ϶��
		if (!m_Modal)
		{
			ImGui::Begin(strFullName.c_str(), &m_Active);

			// �����Լ� render_update ȣ��, override�� �ڽ��� render�� ȣ��� ��
			render_update();

			// �ڽ�UI finaltick ȣ��
			for (size_t i = 0; i < m_vecChildUI.size(); ++i)
			{
				// �ڽ�UI�� ��Ȱ��ȭ ���¸� �ǳʶڴ�.
				if (!m_vecChildUI[i]->IsActive())
					continue;

				m_vecChildUI[i]->finaltick();

				// ������ �ڽ�UI�� �ƴ϶�� ���м� �߱�
				if (i != m_vecChildUI.size() - 1)
					ImGui::Separator();
			}

			ImGui::End();
		}
		// ����̸�
		else
		{
			ImGui::SetNextWindowPos(m_vPopupPos);
			ImGui::SetNextWindowSize(m_vSize);

			ImGui::OpenPopup(strFullName.c_str());
			if(ImGui::BeginPopupModal(strFullName.c_str(), &m_Active))
			{
				render_update();
				
				// �ڽ�UI finaltick ȣ��
				for (size_t i = 0; i < m_vecChildUI.size(); ++i)
				{
					// �ڽ�UI�� ��Ȱ�� ���¸� �ǳʶڴ�
					if (!m_vecChildUI[i]->IsActive())
						continue;

					m_vecChildUI[i]->finaltick();

					// ���м�
					if (i != m_vecChildUI.size() - 1)
						ImGui::Separator();
				}
				ImGui::EndPopup();
			}
		}
	}
	// �ڽĿ�����Ʈ�� ���
	else
	{
		// �ڽ�UI ���� Beginȣ�� �� ũ�� ����
		ImGui::BeginChild(strFullName.c_str(), m_vSize);

		render_update();

		// �ڽ�UI finaltick ȣ��
		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->finaltick();

			// ������ �ڽ�UI�� �ƴ϶�� ���м� �߱�
			if (i != m_vecChildUI.size() - 1)
				ImGui::Separator();
		}

		ImGui::EndChild();
	}
}

