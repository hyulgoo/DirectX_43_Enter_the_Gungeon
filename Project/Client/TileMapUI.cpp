#include "pch.h"
#include "TileMapUI.h"
#include <Engine/CGameObject.h>
#include <Engine/CTileMap.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CResMgr.h>
#include <Engine/CTexture.h>
#include <Engine/CTransform.h>
#include "ListUI.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CAstarMgr.h>


TileMapUI::TileMapUI()
	: ComponentUI("##TileMap", COMPONENT_TYPE::TILEMAP)
	, m_CurTileInfo{}
	, m_EditTileInfo{}
	, m_SelectTile {}
	, m_bEditState(false)
	, m_iTileState((UINT)TILESTATE::NONE)
	, m_bShowFrame(false)
{
	SetName("TileMap");
}

TileMapUI::~TileMapUI()
{
}

int TileMapUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;
	static ImVec2 Slice = ImVec2(0.1f, 0.1f);
	static int TileCount[2];

	if (nullptr == GetTarget())
		m_CurTileInfo.TileMap = nullptr;
	// 타일을 가져옴
	if (nullptr == m_CurTileInfo.TileMap && nullptr != GetTarget())
	{
		m_CurTileInfo.TileMap = GetTarget()->TileMap();
		m_CurTileInfo.TileMap->begin();
		m_CurTileInfo.Atlas = m_CurTileInfo.TileMap->GetAtlas();
		m_CurTileInfo.Slice = ImVec2(m_CurTileInfo.TileMap->GetSliceSize().x, m_CurTileInfo.TileMap->GetSliceSize().y);
		m_CurTileInfo.XCount = (UINT)m_CurTileInfo.TileMap->GetTileCount().x;
		m_CurTileInfo.YCount = (UINT)m_CurTileInfo.TileMap->GetTileCount().y;
		TileCount[0] = m_CurTileInfo.XCount;
		TileCount[1] = m_CurTileInfo.YCount;
	}

	m_CurTileInfo.TileMap = GetTarget()->TileMap();
	m_CurTileInfo.TileMap = GetTarget()->TileMap();
	m_CurTileInfo.Atlas = m_CurTileInfo.TileMap->GetAtlas();
	m_CurTileInfo.Slice = ImVec2(m_CurTileInfo.TileMap->GetSliceSize().x, m_CurTileInfo.TileMap->GetSliceSize().y);
	m_CurTileInfo.XCount = (UINT)m_CurTileInfo.TileMap->GetTileCount().x;
	m_CurTileInfo.YCount = (UINT)m_CurTileInfo.TileMap->GetTileCount().y;

	// 아틀라스 이름 및 선택기능
	if(nullptr != m_CurTileInfo.Atlas)
	{
		string animname = string(m_CurTileInfo.Atlas->GetKey().begin(), m_CurTileInfo.Atlas->GetKey().end());
		ImGui::Text("Texture  ");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60.f);
		ImGui::InputText("##Atlas Name", (char*)animname.c_str(), 50, ImGuiInputTextFlags_ReadOnly);
		ImGui::SameLine();
		map<wstring, Ptr<CRes>> AllTex = CResMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);
		if (ImGui::Button("##AtlasSelectBtn", ImVec2(18.f, 18.f)))
		{
			ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
			pListUI->SetModal(false);
			pListUI->Reset("Atlas List", ImVec2(300.f, 500.f));
			for (const auto& pair : AllTex)
			{
				pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
			}

			pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&TileMapUI::SelectAtlas);
		}
	}
	ImGui::Text("ShowFrame");
	ImGui::SameLine(); 
	if (ImGui::Checkbox("##FrameCheck", &m_bShowFrame))
		m_CurTileInfo.TileMap->SetDrawFrame();

	ImGui::Text("SliceSize");
	ImGui::SameLine();
	ImGui::InputFloat2("##SliceSize", &Slice.x);

	ImGui::Text("TileCount");
	ImGui::SameLine();
	ImGui::InputInt2("##TileCount", &TileCount[0]);

	ImGui::Text("ClearTile");
	ImGui::SameLine();
	if(ImGui::Button(("##ClearTile"), ImVec2(20.f, 20.f)))
	{
		m_CurTileInfo.TileMap->ClearTile();
	}


	if (ImGui::Button(("APPLY"), ImVec2(100.f, 25.f)))
	{
		m_CurTileInfo.TileMap->Transform()->SetRelativeScale(Vec3(TileCount[0] * Slice.x * m_CurTileInfo.Atlas->Width()
															, TileCount[1] * Slice.y * m_CurTileInfo.Atlas->Height(), 1.f));
		m_CurTileInfo.TileMap->SetSliceSize(Vec2(Slice.x, Slice.y));
		m_CurTileInfo.TileMap->SetTileCount(TileCount[0], TileCount[1]);
	}

	ImGui::SameLine();
	static int clicked = 0;
	if (ImGui::Button(("TILE EDIT"), ImVec2(100.f, 25.f)))
		clicked++;
	if (clicked & 1)
	{
		ShowTileEdit();
	}

	if (ImGui::Button(("SAVE TILE"), ImVec2(100.f, 25.f)))
	{
		m_CurTileInfo.TileMap->Save(L"Tile\\Tile.tile");
	}
	ImGui::SameLine();
	if (ImGui::Button(("LOAD TILE"), ImVec2(100.f, 25.f)))
	{
		//m_CurTileInfo.TileMap->SetTilePos();
		m_CurTileInfo.TileMap->Load(L"Tile\\Tile.tile");
	}
	if (ImGui::Button(("AWAW TILE"), ImVec2(100.f, 25.f)))
	{
		m_CurTileInfo.TileMap->Load(L"Tile\\Tile.tile");
		//m_CurTileInfo.TileMap->SetTilePos();
		//CAstarMgr::GetInst()->LoadTile(m_CurTileInfo.TileMap);
	}

	if (ImGui::Button(("SAVE WALL"), ImVec2(100.f, 25.f)))
	{
		m_CurTileInfo.TileMap->Save(L"Tile\\Wall.tile");
	}
	ImGui::SameLine();
	if (ImGui::Button(("LOAD WALL"), ImVec2(100.f, 25.f)))
	{
		m_CurTileInfo.TileMap->Load(L"Tile\\Wall.tile"); 
		m_CurTileInfo.TileMap->SetTilePos();
	}

	

	return TRUE;
}

void TileMapUI::ShowTileEdit()
{
	ImGui::Begin("TileEditor", 0, ImGuiWindowFlags_NoScrollbar);
	for (UINT i = 0; i < 10; ++i)
	{
		for (UINT j = 0; j < 10; ++j)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
			ImVec2 uv0 = ImVec2(j * 0.1f, i * 0.1f);
			ImVec2 uv1 = ImVec2(uv0.x + 0.1f, uv0.y + 0.1f);
			m_mapTile.insert(make_pair(j + (i * 10) + 1, tTile{ Vec2(uv0.x, uv0.y), Vec2(0.1f, 0.1f) }));
			ImGui::ImageButton(m_CurTileInfo.Atlas->GetSRV().Get(), ImVec2((ImGui::GetWindowSize().x * 0.8f) / 10, (ImGui::GetWindowSize().y * 0.8f) / 10)
				, uv0, uv1, -1, ImVec4(0.f, 0.f, 0.f, 0.f), ImVec4(1.f, 1.f, 1.f, 0.9f));
			if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
			{
				m_SelectTile = m_mapTile.find(j + (i * 10) + 1)->second;
			}
			if (j < 10 - 1)
				ImGui::SameLine(0);
			ImGui::PopStyleVar();
		}
	}
	ImGui::End();

	EditCurTile();
}

void TileMapUI::EditCurTile()
{
	// 타일 기본 정보
	Vec3 MousePos = CRenderMgr::GetInst()->GetRay();
	Vec3 TilePos = m_CurTileInfo.TileMap->Transform()->GetWorldPos();
	ImGui::InputFloat2("TilePos", &TilePos.x);
	Vec3 TileScale = m_CurTileInfo.TileMap->Transform()->GetRelativeScale();
	ImGui::InputFloat2("TileScale", &TileScale.x);
	Vec2 TileLtp = Vec2(TilePos.x - TileScale.x / 2, TilePos.y - TileScale.y / 2);
	ImGui::InputFloat2("TileLtp", &TileLtp.x);
	Vec2 TileBtm = Vec2(TilePos.x + TileScale.x / 2, TilePos.y + TileScale.y / 2);
	ImGui::InputFloat2("TileBtm", &TileBtm.x);
	int iTileState = (int)m_iTileState;
	ImGui::Text("StateType");
	ImGui::SameLine();
	ImGui::InputInt("##StateType", &iTileState);
	m_iTileState = (UINT)iTileState;

	// 타일 이동 가능상태를 어떤 모드로 수정할 지
	if (KEY_TAP(KEY::_1))
	{
		m_iTileState = (UINT)TILESTATE::NONE;
	}
	if (KEY_TAP(KEY::_2))
	{
		m_iTileState = (UINT)TILESTATE::ABLE;
	}
	if (KEY_TAP(KEY::_3))
	{
		m_iTileState = (UINT)TILESTATE::DISABLE;
	}
	if (KEY_TAP(KEY::O))
		m_bEditState = m_bEditState ? false : true;
	ImGui::Text("StateMode");
	ImGui::SameLine();
	ImGui::Checkbox("##StateMode", &m_bEditState);

	// 타일 이미지 수정 모드
	if(!m_bEditState)
	{
		if (KEY_PRESSED(KEY::LBTN) && KEY_PRESSED(KEY::LCTRL))
		{
			if (TileLtp.x < MousePos.x && MousePos.x < TileBtm.x
				&& TileLtp.y < -MousePos.y && -MousePos.y < TileBtm.y)
			{
				Vec2 MouseAtTile = Vec2(abs(MousePos.x - TileLtp.x), abs(-MousePos.y - TileLtp.y));
				int X = MouseAtTile.x / (m_CurTileInfo.Slice.x * m_CurTileInfo.Atlas->Width());
				int Y = MouseAtTile.y / (m_CurTileInfo.Slice.y * m_CurTileInfo.Atlas->Height());
				int TileNum = X + (Y * m_CurTileInfo.XCount);
				if (TileNum < m_CurTileInfo.XCount * m_CurTileInfo.YCount)
				{
					m_CurTileInfo.TileMap->SetTile(TileNum, m_SelectTile);
				}
			}
		}
	}
	// 타일 이동 가능상태 수정 모드
	else if (m_bEditState)
	{
		if (KEY_PRESSED(KEY::LBTN) && KEY_PRESSED(KEY::LCTRL))
		{
			if (TileLtp.x < MousePos.x && MousePos.x < TileBtm.x
				&& TileLtp.y < -MousePos.y && -MousePos.y < TileBtm.y)
			{
				Vec2 MouseAtTile = Vec2(abs(MousePos.x - TileLtp.x), abs(-MousePos.y - TileLtp.y));
				int X = MouseAtTile.x / (m_CurTileInfo.Slice.x * m_CurTileInfo.Atlas->Width());
				int Y = MouseAtTile.y / (m_CurTileInfo.Slice.y * m_CurTileInfo.Atlas->Height());
				int TileNum = X + (Y * m_CurTileInfo.XCount);
				if (TileNum < m_CurTileInfo.XCount * m_CurTileInfo.YCount)
				{
					m_CurTileInfo.TileMap->SetTileState(TileNum, (TILESTATE)m_iTileState);
					tTile SetTile = m_CurTileInfo.TileMap->GetTile(TileNum);

				}
			}
		}
	}
}

void TileMapUI::SelectAtlas(DWORD_PTR _Key)
{
	string strKey = (char*)_Key;
	wstring AtlasName = wstring(strKey.begin(), strKey.end());
	Ptr<CTexture>SeletedTex = CResMgr::GetInst()->FindRes<CTexture>(AtlasName);
	m_CurTileInfo.TileMap->GetMaterial()->SetTexParam(TEX_0, SeletedTex);
	m_CurTileInfo.Atlas = SeletedTex;
}

