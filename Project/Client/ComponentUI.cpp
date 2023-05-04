#include "pch.h"
#include "ComponentUI.h"

#include <Engine/CGameObject.h>

ComponentUI::ComponentUI(const string& _Name, COMPONENT_TYPE _Type)
	: UI(_Name)
	, m_Type(_Type)
	, m_Target(nullptr)
{
}

ComponentUI::~ComponentUI()
{
}

int ComponentUI::render_update()
{
	if (nullptr == m_Target || nullptr == m_Target->GetComponent(m_Type))
		return FALSE;

	// UI 디자인
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
	ImGui::Button(GetName().c_str());
	ImGui::PopStyleColor(3);
	ImGui::PopID();

	return TRUE;
}

void ComponentUI::SetTarget(CGameObject* _Target)
{
	m_Target = _Target;

	if (nullptr == m_Target)
	{
		SetActive(false);
		return;
	}
	// Target이 UI가 담당하는 Component를 가지고 있지 않다면 비활성화
	if (nullptr == m_Target->GetComponent(m_Type))
	{		
		SetActive(false); 
	}
	// Component를 가지고 있다면 활성화함
	else
	{
		SetActive(true);
	}
}

void ComponentUI::GetResKey(Ptr<CRes> _Res, char* _Buff, size_t _BufferSize)
{
	memset(_Buff, 0, sizeof(char) * _BufferSize);

	if (nullptr == _Res)
		return;

	wstring wstrKey = _Res->GetKey();
	string	strKey = string(wstrKey.begin(), wstrKey.end());
	memcpy(_Buff, strKey.data(), sizeof(char) * strKey.length());
}
