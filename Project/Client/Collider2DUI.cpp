#include "pch.h"
#include "Collider2DUI.h"
#include <Engine/CCollider2D.h>
#include <Engine/CRenderMgr.h>
#include <Script/CWaveScript.h>

Collider2DUI::Collider2DUI()
	: ComponentUI("##Collider2D", COMPONENT_TYPE::COLLIDER2D)
	, m_bShowEdieWave(false)
{
	SetName("Collider2D");
}

Collider2DUI::~Collider2DUI()
{
}

int Collider2DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	Vec3 vScale = GetTarget()->Collider2D()->GetOffsetScale();
	Vec3 vPos = GetTarget()->Collider2D()->GetOffsetPos();
	bool bAbs = GetTarget()->Collider2D()->IsAbsolute();
	int iabs = bAbs ? 1 : 0;
	COLLIDER2D_TYPE tShape = GetTarget()->Collider2D()->GetCollider2DType();

	ImGui::Text("Position");
	ImGui::SameLine();
	ImGui::DragFloat3("##Offset Pos", vPos);


	ImGui::Text("Scale   ");
	ImGui::SameLine();
	ImGui::DragFloat3("##Offset Scale", vScale);


	ImGui::Text("Absolute");
	ImGui::SameLine();
	static int elem = iabs;
	const char* elems_names[2] = { "FALSE", "TRUE"};
	const char* elem_name = (elem >= 0 && elem < 2) ? elems_names[elem] : "Unknown";
	ImGui::SliderInt("Absolute", &elem, 0, 1, elem_name);
	bAbs = elem ? true : false;

	ImGui::Text("Type    ");
	ImGui::SameLine();
	const char* item_names[] =
	{
		"RECT", "CIRCLE"
	};
	static int item_type = (int)tShape;
	static bool _disabled = false;
	ImGui::Combo("Collider2D Type", &item_type, item_names, IM_ARRAYSIZE(item_names), IM_ARRAYSIZE(item_names));


	ImGui::Text("Collision Count");
	ImGui::SameLine();
	int iCount = GetTarget()->Collider2D()->GetCollisionCount();
	ImGui::DragInt("##Collision Count", &iCount, ImGuiInputTextFlags_ReadOnly);

	GetTarget()->Collider2D()->SetOffsetScale(Vec2(vScale.x, vScale.y));
	GetTarget()->Collider2D()->SetOffsetPos(Vec2(vPos.x, vPos.y));
	GetTarget()->Collider2D()->SetAbsolute(bAbs);
	GetTarget()->Collider2D()->SetCollider2DType((COLLIDER2D_TYPE)item_type);

	ImGui::Checkbox("##WaveEditor", &m_bShowEdieWave);
	if(m_bShowEdieWave)
		ShowWaveEditor();

    return TRUE;
}

void Collider2DUI::ShowWaveEditor()
{
	ImGui::Begin("Animation Editor", &m_bShowEdieWave, ImGuiWindowFlags_NoDocking);

	CWaveScript* pWave = GetTarget()->GetScript<CWaveScript>();

	string CurNum = std::to_string(m_iPrefabNum);

	ImGui::Text(CurNum.c_str());
	ImGui::SameLine();
	ImGui::InputFloat2("##SpawnPos", &m_vSpawnPos.x);

	if (ImGui::Button("GreenBook      "))
		m_iPrefabNum = (int)MONSTER_TYPE::GREENBOOK;
	ImGui::SameLine();
	if (ImGui::Button("RedBook        "))
		m_iPrefabNum = (int)MONSTER_TYPE::REDBOOK;
	if (ImGui::Button("BulletKin      "))
		m_iPrefabNum = (int)MONSTER_TYPE::BULLETKIN;
	ImGui::SameLine();
	if (ImGui::Button("ShotgunKin_Blue"))
		m_iPrefabNum = (int)MONSTER_TYPE::SHOTGUNKIN_BLUE;
	if (ImGui::Button("ShotgunKin_Red "))
		m_iPrefabNum = (int)MONSTER_TYPE::SHOTGUNKIN_RED;
	ImGui::SameLine();
	if (ImGui::Button("BulletBat      "))
		m_iPrefabNum = (int)MONSTER_TYPE::BULLETBAT;
	if (ImGui::Button("ShotBat        "))
		m_iPrefabNum = (int)MONSTER_TYPE::SHOTBAT;
	if (ImGui::Button("Boss           "))
		m_iPrefabNum = (int)MONSTER_TYPE::GATLING_GULL;



	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::LBTN))
		m_vSpawnPos = CRenderMgr::GetInst()->GetRay();

	ImGui::Text("Wave Num");
	ImGui::SameLine();
	static int WaveNum = 0;
	ImGui::InputInt("##WaveNum", &WaveNum);

	ImGui::Text("Max  Num");
	ImGui::SameLine();
	static int MaxWaveNum = 0;
	ImGui::InputInt("##MaxWaveNum", &MaxWaveNum);

	if (ImGui::Button("ADD Mst", ImVec2(60.f, 20.f)))
		pWave->AddWaveMst(WaveNum, m_iPrefabNum, m_vSpawnPos);

	vector<WaveInfo> Wave0 = pWave->GetWaveInfo(0);
	vector<WaveInfo> Wave1 = pWave->GetWaveInfo(1);
	vector<WaveInfo> Wave2 = pWave->GetWaveInfo(2);

	vector<WaveInfo> WaveInfo;
	switch (WaveNum)
	{
	case 0:
		WaveInfo = Wave0;
		break;
	case 1:
		WaveInfo = Wave1;
		break;
	case 2:
		WaveInfo = Wave2;
		break;
	}

	for (size_t i = 0; i < WaveInfo.size(); ++i)
	{
		int iPrefNum = WaveInfo[i].PrefabNum;
		int a = 0;
		string Count;
		Count = std::to_string(a);
		ImGui::InputInt(Count.c_str(), &iPrefNum);
		string label = "##";
		label += Count;
		ImGui::InputFloat3(label.c_str(), &WaveInfo[i].SpawnPos.x);
		++a;
	}

	pWave->SetWaveCount(MaxWaveNum);
	pWave->SetWaveInfo(0, Wave0);
	pWave->SetWaveInfo(1, Wave1);
	pWave->SetWaveInfo(2, Wave2);

	ImGui::End();
}
