#include "pch.h"
#include "Light2DUI.h"
#include <Engine/CGameObject.h>
#include <Engine/CLight2D.h>

Light2DUI::Light2DUI()
	: ComponentUI("##Light2D", COMPONENT_TYPE::LIGHT2D)
{
	SetName("Light2D");
}

Light2DUI::~Light2DUI()
{
}

int Light2DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	tLightInfo tLightInfo = GetTarget()->Light2D()->GetLightInfo();

	float fAngle = tLightInfo.Angle;
	fAngle *= 180.f / XM_PI;
	Vec4 vDiffuse = tLightInfo.Color.vDiffuse;
	ImVec4 ivDiffuse = ImVec4(vDiffuse.x, vDiffuse.y, vDiffuse.z, vDiffuse.w);

	Vec4 vAmbient = tLightInfo.Color.vAmbient;
	ImVec4 ivAmbient = ImVec4(vAmbient.x, vAmbient.y, vAmbient.z, vAmbient.w);

	UINT iType = tLightInfo.LightType;
	int itype = (int)iType;
	float fRadius = tLightInfo.Radius;
	Vec4 vWorldPos = tLightInfo.vWorldPos;

	ImGui::Text("Angle  ");
	ImGui::SameLine();
	ImGui::DragFloat("##Angle", &fAngle);


	ImGui::Text("Diffuse");
	ImGui::SameLine();
	ImGui::DragFloat4("##Diffuse", &ivDiffuse.x);


	ImGui::Text("Ambient");
	ImGui::SameLine();
	ImGui::DragFloat4("##Ambient", &ivAmbient.x);

	ImGui::Text("Type   ");
	ImGui::SameLine();
	ImGui::DragInt("##Ambient", &itype);

	ImGui::Text("Radius ");
	ImGui::SameLine();
	ImGui::DragFloat("##Radius", &fRadius);

	vDiffuse = Vec4(ivDiffuse.x, ivDiffuse.y, ivDiffuse.z, ivDiffuse.w);
	vAmbient = Vec4(ivAmbient.x, ivAmbient.y, ivAmbient.z, ivAmbient.w);
	iType = (UINT)itype;
	fAngle *= XM_PI / 180.f;

	GetTarget()->Light2D()->SetAngle(fAngle);
	GetTarget()->Light2D()->SetLightDiffuse(vDiffuse);
	GetTarget()->Light2D()->SetLightAmbient(vAmbient);
	GetTarget()->Light2D()->SetRadius(fRadius);
	GetTarget()->Light2D()->SetLightType((LIGHT_TYPE)iType);

	return TRUE;
}
