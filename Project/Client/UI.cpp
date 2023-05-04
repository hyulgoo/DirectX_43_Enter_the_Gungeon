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

	// 자신이 부모UI라면
	if (nullptr == m_ParentUI)
	{
		// 모달방식이 아니라면
		if (!m_Modal)
		{
			ImGui::Begin(strFullName.c_str(), &m_Active);

			// 가상함수 render_update 호출, override한 자식의 render가 호출될 것
			render_update();

			// 자식UI finaltick 호출
			for (size_t i = 0; i < m_vecChildUI.size(); ++i)
			{
				// 자식UI가 비활성화 상태면 건너뛴다.
				if (!m_vecChildUI[i]->IsActive())
					continue;

				m_vecChildUI[i]->finaltick();

				// 마지막 자식UI가 아니라면 구분선 긋기
				if (i != m_vecChildUI.size() - 1)
					ImGui::Separator();
			}

			ImGui::End();
		}
		// 모달이면
		else
		{
			ImGui::SetNextWindowPos(m_vPopupPos);
			ImGui::SetNextWindowSize(m_vSize);

			ImGui::OpenPopup(strFullName.c_str());
			if(ImGui::BeginPopupModal(strFullName.c_str(), &m_Active))
			{
				render_update();
				
				// 자식UI finaltick 호출
				for (size_t i = 0; i < m_vecChildUI.size(); ++i)
				{
					// 자식UI가 비활성 상태면 건너뛴다
					if (!m_vecChildUI[i]->IsActive())
						continue;

					m_vecChildUI[i]->finaltick();

					// 구분선
					if (i != m_vecChildUI.size() - 1)
						ImGui::Separator();
				}
				ImGui::EndPopup();
			}
		}
	}
	// 자식오브젝트일 경우
	else
	{
		// 자식UI 전용 Begin호출 및 크기 지정
		ImGui::BeginChild(strFullName.c_str(), m_vSize);

		render_update();

		// 자식UI finaltick 호출
		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->finaltick();

			// 마지막 자식UI가 아니라면 구분선 긋기
			if (i != m_vecChildUI.size() - 1)
				ImGui::Separator();
		}

		ImGui::EndChild();
	}
}

