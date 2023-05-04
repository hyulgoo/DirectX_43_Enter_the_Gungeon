#include "pch.h"
#include "ParticleSystemUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CParticleSystem.h>
#include "TreeUI.h"

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI("##ParticleSystemUI", COMPONENT_TYPE::PARTICLESYSTEM)
	, m_pParticleSystem(nullptr)
	, m_tModuleData{}
{
	SetName("ParticleSysyem");
}

ParticleSystemUI::~ParticleSystemUI()
{
}

int ParticleSystemUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	if (nullptr == m_pParticleSystem)
	{
		m_pParticleSystem = GetTarget()->ParticleSystem();
	}
	m_tModuleData = m_pParticleSystem->GetModuleData();

	ONOFF("Spawn Module", "##Spawn_Module", m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN]);
	if (1 == m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN])
		SpawnModule();

	ImGui::NewLine();
	ImGui::Separator();

	ONOFF("Scale Change", "##Scale_Change", m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE]);
	if (1 == m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE])
		ScaleModule();

	ImGui::NewLine();
	ImGui::Separator();

	ONOFF("Color Change", "##Color_Change", m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE]);
	if (1 == m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE])
		ColorModule();

	ImGui::NewLine();
	ImGui::Separator();

	ONOFF("Add Velocity", "##Add_Velocity", m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY]);
	if (1 == m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY])
		AddVelocityModule();

	ImGui::NewLine();
	ImGui::Separator();

	ONOFF("Drag        ", "##Drag_Module", m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG]);
	if (1 == m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG])
		DragModule();

	ImGui::NewLine();
	ImGui::Separator();

	ONOFF("Random Force", "##Random_Force", m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RANDOM_FORCE]);
	if (1 == m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RANDOM_FORCE])
		RandomForceModule();

	ImGui::NewLine();
	ImGui::Separator();

	ONOFF("VelocityAlignment", "##Velocity_Alignment", m_tModuleData.VelocityAlignment);

	ImGui::NewLine();
	ImGui::Separator();

	ONOFF("VelocityScale", "##Velocity_Scale", m_tModuleData.VelocityScale);
	if (1 == m_tModuleData.VelocityScale)
		VelocityScaleModule();

	ImGui::NewLine();
	ImGui::Separator();

	ONOFF("Animation    ", "##Particle_Animation", m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ANIMATION]);
	if (1 == m_tModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ANIMATION])
		AnimationModule();


	m_pParticleSystem->SetModuleData(m_tModuleData);

	return TRUE;
}

void ParticleSystemUI::ONOFF(string _strName, string _strID, int& _ModuleCheck)
{
	ImVec4 Color = ImVec4(255.f / 255.f, 105.f / 255.f, 97.f / 255, 1.f);
	if(1 ==_ModuleCheck)
		Color = ImVec4(152.f / 255.f, 251.f / 255.f, 152.f / 255, 1.f);
	ImGui::TextColored(ImVec4(255.f / 255.f, 174.f / 255.f, 105.f / 255, 1.f), _strName.c_str());
	ImGui::SameLine();
	char szbuffer[50] = {"ON/OFF##"};
	strcpy_s(szbuffer, _strName.c_str());
	strcpy_s(szbuffer, _strID.c_str());
	if (ImGui::ColorButton(szbuffer, Color, ImGuiColorEditFlags_::ImGuiColorEditFlags_NoTooltip, ImVec2(15.f, 15.f)))
	{
		_ModuleCheck = _ModuleCheck ? false : true;
	}

}

void ParticleSystemUI::SpawnModule()
{
	ImGui::Text("MaxCount    ");
	ImGui::SameLine();
	ImGui::DragInt("##Spawn_MaxCount", &m_tModuleData.iMaxParticleCount);

	ImGui::Text("SpawnRate   ");
	ImGui::SameLine();
	ImGui::DragInt("##Spawn_SpawnRate", &m_tModuleData.SpawnRate);

	//ImVec4 SpawnColor = ImVec4(m_tModuleData.vSpawnColor.x, m_tModuleData.vSpawnColor.y, m_tModuleData.vSpawnColor.z, m_tModuleData.vSpawnColor.z);
	ImGui::Text("Color       ");
	ImGui::SameLine();
	ImGui::DragFloat4("##Spawn_Color", &m_tModuleData.vSpawnColor.x);
	//m_tModuleData.vSpawnColor = Vec4(SpawnColor.x, SpawnColor.y, SpawnColor.z, SpawnColor.w);

	ImGui::Text("MinScale    ");
	ImGui::SameLine();
	ImGui::DragFloat3("##Spawn_MinScale", &m_tModuleData.vSpawnScaleMin.x);

	ImGui::Text("MaxScale    ");
	ImGui::SameLine();
	ImGui::DragFloat3("##Spawn_MaxScale", &m_tModuleData.vSpawnScaleMax.x);

	ImGui::Text("ShapeType   ");
	ImGui::SameLine();
	int elem = m_tModuleData.SpawnShapeType;
	const char* elems_names[2] = { "BOX", "SPHERE" };
	const char* elem_name = (elem >= 0 && elem < 2) ? elems_names[elem] : "Unknown";
	ImGui::SliderInt("##Spawn_ShapeType", &m_tModuleData.SpawnShapeType, 0, 1, elem_name);

	if (0 == m_tModuleData.SpawnShapeType)
	{
		ImGui::Text("SpawnRange  ");
		ImGui::SameLine();
		ImGui::DragFloat3("##Spawn_BoxRange", &m_tModuleData.vBoxShapeScale.x);
	}

	else if (1 == m_tModuleData.SpawnShapeType)
	{
		ImGui::Text("SpawnRange  ");
		ImGui::SameLine();
		ImGui::DragFloat("##Spawn_SpheheRadius", &m_tModuleData.fSphereShapeRadius);
	}

	ImGui::Text("MinTime     ");
	ImGui::SameLine();
	ImGui::DragFloat("##Spawn_MinTime", &m_tModuleData.MinLifeTime);

	ImGui::Text("MaxTime     ");
	ImGui::SameLine();
	ImGui::DragFloat("##Spawn_MaxTime", &m_tModuleData.MaxLifeTime);

	Ptr<CTexture> Tex = GetTarget()->ParticleSystem()->GetTex();
	char szbuff[50] = {};
	
	
	(Tex.Get(), szbuff, 50);
	ImGui::Text("Texture     ");
	ImGui::SameLine();
	ImGui::InputText("##TexName", szbuff, ImGuiInputTextFlags_ReadOnly);
}

void ParticleSystemUI::ScaleModule()
{
	ImGui::Text("StartScale ");
	ImGui::SameLine();
	ImGui::DragFloat("##Start_Scale", &m_tModuleData.StartScale);

	ImGui::Text("EndScale   ");
	ImGui::SameLine();
	ImGui::DragFloat("##End_Scale", &m_tModuleData.EndScale);
}

void ParticleSystemUI::ColorModule()
{
	ImGui::Text("StartColor  ");
	ImGui::SameLine();
	// Generate a default palette. The palette will persist and can be edited.
	static bool saved_palette_init = true;
	static ImVec4 saved_palette[32] = {};
	if (saved_palette_init)
	{
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
		{
			ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
				saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
			saved_palette[n].w = 1.0f; // Alpha
		}
		saved_palette_init = false;
	}

	static ImVec4 color = ImVec4(m_tModuleData.vStartColor.x, m_tModuleData.vStartColor.y, m_tModuleData.vStartColor.z, m_tModuleData.vStartColor.w);
	ImGuiColorEditFlags misc_flags = ImGuiColorEditFlags_HDR;
	static ImVec4 backup_color;
	bool open_popup = ImGui::ColorButton("MyColor##1", color, misc_flags);
	if (open_popup)
	{
		ImGui::OpenPopup("mypicker##1");
		backup_color = ImVec4(m_tModuleData.vStartColor.x, m_tModuleData.vStartColor.y, m_tModuleData.vStartColor.z, m_tModuleData.vStartColor.w);
	}
	if (ImGui::BeginPopup("mypicker##1"))
	{
		ImGui::Text("START COLOR PALETTE");
		ImGui::Separator();
		ImGui::ColorPicker4("##picker1", (float*)&color, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
		ImGui::SameLine();

		ImGui::BeginGroup(); // Lock X position
		ImGui::Text("Current##1");
		ImGui::ColorButton("##current1", color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
		ImGui::Text("Previous##1");
		if (ImGui::ColorButton("##previous1", backup_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40)))
			color = backup_color;
		ImGui::Separator();
		ImGui::Text("Palette##1");
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
		{
			ImGui::PushID(n);
			if ((n % 8) != 0)
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);

			ImGuiColorEditFlags palette_button_flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip;
			if (ImGui::ColorButton("##palette1", saved_palette[n], palette_button_flags, ImVec2(20, 20)))
				color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!

			// Allow user to drop colors into each palette entry. Note that ColorButton() is already a
			// drag source by default, unless specifying the ImGuiColorEditFlags_NoDragDrop flag.
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
					memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 3);
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
					memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 4);
				ImGui::EndDragDropTarget();
			}

			ImGui::PopID();
		}
		ImGui::EndGroup();
		ImGui::EndPopup();
		m_tModuleData.vStartColor = Vec4(color.x, color.y, color.z, color.w);
	}

	ImGui::Text("EndColor    ");
	ImGui::SameLine();

	// Generate a default palette. The palette will persist and can be edited.
	static bool saved_palette_init1 = true;
	static ImVec4 saved_palette1[32] = {};
	if (saved_palette_init1)
	{
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette1); n++)
		{
			ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
				saved_palette1[n].x, saved_palette1[n].y, saved_palette1[n].z);
			saved_palette1[n].w = 1.0f; // Alpha
		}
		saved_palette_init1 = false;
	}

	static ImVec4 color1 = ImVec4(m_tModuleData.vEndColor.x, m_tModuleData.vEndColor.y, m_tModuleData.vEndColor.z, m_tModuleData.vEndColor.w);
	ImGuiColorEditFlags misc_flags1 = ImGuiColorEditFlags_HDR;
	static ImVec4 backup_color1;
	bool open_popup1 = ImGui::ColorButton("MyColor##2", color1, misc_flags1);
	if (open_popup1)
	{
		ImGui::OpenPopup("mypicker##2");
		backup_color1 = ImVec4(m_tModuleData.vEndColor.x, m_tModuleData.vEndColor.y, m_tModuleData.vEndColor.z, m_tModuleData.vEndColor.w);
	}
	if (ImGui::BeginPopup("mypicker##2"))
	{
		ImGui::Text("END COLOR PALETTE");
		ImGui::Separator();
		ImGui::ColorPicker4("##picker2", (float*)&color1, misc_flags1 | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
		ImGui::SameLine();

		ImGui::BeginGroup(); // Lock X position
		ImGui::Text("Current##2");
		ImGui::ColorButton("##current2", color1, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
		ImGui::Text("Previous##2");
		if (ImGui::ColorButton("##previous2", backup_color1, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40)))
			color1 = backup_color1;
		ImGui::Separator();
		ImGui::Text("Palette##2");
		for (int n = 0; n < IM_ARRAYSIZE(saved_palette1); n++)
		{
			ImGui::PushID(n);
			if ((n % 8) != 0)
				ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);

			ImGuiColorEditFlags palette_button_flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip;
			if (ImGui::ColorButton("##palette2", saved_palette1[n], palette_button_flags, ImVec2(20, 20)))
				color1 = ImVec4(saved_palette1[n].x, saved_palette1[n].y, saved_palette1[n].z, color1.w); // Preserve alpha!

			// Allow user to drop colors into each palette entry. Note that ColorButton() is already a
			// drag source by default, unless specifying the ImGuiColorEditFlags_NoDragDrop flag.
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
					memcpy((float*)&saved_palette1[n], payload->Data, sizeof(float) * 3);
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
					memcpy((float*)&saved_palette1[n], payload->Data, sizeof(float) * 4);
				ImGui::EndDragDropTarget();
			}

			ImGui::PopID();
		}
		ImGui::EndGroup();
		ImGui::EndPopup();
		m_tModuleData.vEndColor = Vec4(color1.x, color1.y, color1.z, color1.w);
	}
}

void ParticleSystemUI::AddVelocityModule()
{
	ImGui::Text("VelocityType");
	ImGui::SameLine();
	int elem = m_tModuleData.AddVelocityType;
	const char* elems_names[3] = { "From Center", "To Center", "Fixed Direction"};
	const char* elem_name = (elem >= 0 && elem < 3) ? elems_names[elem] : "Unknown";
	ImGui::SliderInt("##Velocity_Type", &m_tModuleData.AddVelocityType, 0, 2, elem_name);

	ImGui::Text("Speed       ");
	ImGui::SameLine();
	ImGui::DragFloat("##VelocitySpeed", &m_tModuleData.Speed);

	ImGui::Text("VelocityDir ");
	ImGui::SameLine();
	ImGui::DragFloat4("##VelocityDir", &m_tModuleData.vVelocityDir.x);

	ImGui::Text("OffsetAngle ");
	ImGui::SameLine();
	ImGui::DragFloat("##Offset_Angle", &m_tModuleData.OffsetAngle);
}

void ParticleSystemUI::DragModule()
{
	ImGui::Text("StartDrag   ");
	ImGui::SameLine();
	ImGui::DragFloat("##Start_Drag", &m_tModuleData.StartDrag);

	ImGui::Text("EndDrag     ");
	ImGui::SameLine();
	ImGui::DragFloat("##End_Drag", &m_tModuleData.EndDrag);
}

void ParticleSystemUI::RandomForceModule()
{
	ImGui::Text("Noise Term  ");
	ImGui::SameLine();
	ImGui::DragFloat("##Noise_Term", &m_tModuleData.fNoiseTerm);

	ImGui::Text("Noise Force ");
	ImGui::SameLine();
	ImGui::DragFloat("##Noise_Force", &m_tModuleData.fNoiseForce);
}

void ParticleSystemUI::VelocityScaleModule()
{
	ImGui::Text("MaxScale    ");
	ImGui::SameLine();
	ImGui::DragFloat3("##MaxVelocityScale", &m_tModuleData.vMaxVelocityScale.x);

	ImGui::Text("MaxSpeed    ");
	ImGui::SameLine();
	ImGui::DragFloat("##MaxSpeed", &m_tModuleData.MaxSpeed);
}

void ParticleSystemUI::AnimationModule()
{
	Ptr<CTexture> Atlas = m_pParticleSystem->GetTex();
	Vec2 TexSize = Vec2(Atlas->Width(), Atlas->Height());
	
	ImGui::Text("AnimInfo  ");
	
	Vec2 CurLeftTop = m_tModuleData.vLeftTop * TexSize;
	Vec2 CurSlice = m_tModuleData.vSlice * TexSize;
	Vec2 CurOffset = m_tModuleData.vOffset * TexSize;

	ImGui::Text("FrmNum    ");
	ImGui::SameLine();
	ImGui::DragInt("##AnimFrmNum", &m_tModuleData.iFrmCount, 1.f);

	ImGui::Text("XCount    ");
	ImGui::SameLine();
	ImGui::DragInt("##AnimXCount", &m_tModuleData.iXCount, 1.f);
		
	ImGui::Text("LeftTop   ");
	ImGui::SameLine();
	ImGui::DragFloat2("##AnimLeftTop", &CurLeftTop.x);
		
	ImGui::Text("Slice     ");
	ImGui::SameLine();
	ImGui::DragFloat2("##AnimSlice", &CurSlice.x);
		
	ImGui::Text("Offset    ");
	ImGui::SameLine();
	ImGui::DragFloat2("##AnimOffset", &CurOffset.x);

	m_tModuleData.vLeftTop = CurLeftTop / TexSize;
	m_tModuleData.vSlice = CurSlice / TexSize;
	m_tModuleData.vOffset = CurOffset / TexSize;
}
