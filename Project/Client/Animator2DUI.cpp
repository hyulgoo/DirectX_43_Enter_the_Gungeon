#include "pch.h"
#include "Animator2DUI.h"
#include <Engine/CTexture.h>
#include <Engine/CResMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CAnim2D.h>
#include <Engine/CKeyMgr.h>
#include "ListUI.h"
#include <commdlg.h>

Animator2DUI::Animator2DUI()
	: ComponentUI("##Animator2D", COMPONENT_TYPE::ANIMATOR2D)
	, m_mapStrData{}
	, m_vecAllFrm{}
	, m_mapAllAnim{}
	, m_PrevAllAnim{}
	, m_vecNewAnim{}
	, m_iNewAnimFrmNum(0)
	, m_NewAnimLtp{}
	, m_NewAnimSlice{}
	, m_NewAnimFPS(1)
	, m_NewAnimBackSize{}
	, m_ShowEditor(false)
	, m_ShowCreator(false)
	, m_CurAnim{}
	, m_CurFrm{} 
	, m_AnimAtlas(nullptr)
	, m_DragLtp{}
	, m_DragBtm{}
	, m_CreateTexSize{} 
	, m_CreateFrmInfo{}
	, m_CreateFrmLtp{}
	, m_CreateFrmBtm{}
	, m_CreateFrmSize{}
{
	SetName("Animator2D");
	m_CreateTexSize = ImVec2(500.f, 500.f);
	
}

Animator2DUI::~Animator2DUI()
{
	//if (nullptr != m_CurAnim)
	//	delete m_CurAnim;
	//Safe_Del_Map(m_mapAllAnim);
}

int Animator2DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	SetAnimInfo();

	if (KEY_TAP(KEY::SPACE))
		m_CurAnim->PlayStop();


    ShowAnimList(m_ShowEditor);

	if (m_ShowEditor)
		ShowAnimEditor(m_ShowEditor);

	EditCurFrm();

	return TRUE;
}

void Animator2DUI::SetAnimInfo()
{
	m_mapAllAnim = GetTarget()->Animator2D()->GetAllAnim();
	m_CurAnim = GetTarget()->Animator2D()->GetCurAnim();
	m_vecAllFrm = m_CurAnim->GetAllFrm();
	if (m_PrevAllAnim.empty())
		m_PrevAllAnim = m_CurAnim->GetAllFrm();
	m_AnimAtlas = m_CurAnim->GetAtlasTex();
	m_CurFrm = m_CurAnim->GetCurFrame();
	m_AtlasRatio = m_AnimAtlas->Height() / m_AnimAtlas->Width();

}

void Animator2DUI::ShowAnimList(bool& _ShowEditor)
{
	ImGui::Text("CurAnim ");
	ImGui::SameLine();
	char* szBuff= {};
	string animname = string(m_CurAnim->GetName().begin(), m_CurAnim->GetName().end());
	ImGui::InputText("##CurAnim Name", (char*)animname.c_str(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if(ImGui::Button("##TexSelectBtn", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->SetModal(false);
		pListUI->Reset("Anim List", ImVec2(300.f, 500.f));
		for (const auto& pair : m_mapAllAnim)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}

		// 항목 선택 시 호출될 델리게이트 등록
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&Animator2DUI::SelectAnim);
	}

	if(ImGui::Button("EDITOR", ImVec2(60.0f, 0.0f)))
	{
		_ShowEditor = true;
		m_CurAnim->Stop(0);
	}
}

void Animator2DUI::ShowAnimEditor(bool& _bShow)
{
	ImGui::Begin("Animation Editor", &_bShow, ImGuiWindowFlags_NoDocking );

	// 현재 애니메이션 및 리스트
	ImGui::Text("CurAnim ");
	ImGui::SameLine();
	char* AnimName = {};
	string animname = string(m_CurAnim->GetName().begin(), m_CurAnim->GetName().end());
	ImGui::InputText("##AnimName", (char*)animname.c_str(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("##AnimSelectBtn", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->SetModal(false);
		pListUI->Reset("Anim List", ImVec2(300.f, 500.f));
		for (const auto& pair : m_mapAllAnim)
		{
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}

		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&Animator2DUI::SelectAnim);
	}

	// 현재 아틀라스 및 리스트
	char AtlasName[50] = {};
	GetResKey(m_AnimAtlas.Get(), AtlasName, 50);
	ImGui::Text("Atlas   ");
	ImGui::SameLine();
	ImGui::InputText("##AtlasName", AtlasName, 50, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	if (ImGui::Button("##AtlasSelectBtn", ImVec2(18.f, 18.f)))
	{
		const map<wstring, Ptr<CRes>>& AllTex = CResMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);

		ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
		pListUI->SetModal(false);
		pListUI->Reset("Atlas List", ImVec2(300.f, 500.f));
		for (const auto& pair : AllTex)
		{
			if (L"" == pair.first)
				continue;
			pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
		}
		pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&Animator2DUI::SelectAtlas);
	}

	ShowCurAnim();
	ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "NEWANIMATION");
	ImGui::Text("FrmNum  ");
	ImGui::SameLine();
	ImGui::InputInt("##NewAnimFrmNum", &m_iNewAnimFrmNum);
	ImGui::Text("LeftTop ");
	ImGui::SameLine();
	ImGui::InputFloat2("##NewAnimLtp", &m_NewAnimLtp.x);
	ImGui::Text("Slice   ");
	ImGui::SameLine();
	ImGui::InputFloat2("##NewAnimSlice", &m_NewAnimSlice.x);
	ImGui::Text("FPS     ");
	ImGui::SameLine();
	ImGui::InputInt("##NewAnimFPS", &m_NewAnimFPS);
	ImGui::Text("BackSize");
	ImGui::SameLine();
	ImGui::InputFloat2("##NewAnimBackSize", &m_NewAnimBackSize.x);
	
	ImGui::Text("CreateAnimation");
	ImGui::SameLine();
	if (ImGui::Button("##NewAnimCreate", ImVec2(50.f, 20.f)))
	{
		if (m_iNewAnimFrmNum != 0)
		{
			GetTarget()->Animator2D()->DeleteAnim(L"NewAnim");
			GetTarget()->Animator2D()->CreateAnimation(L"NewAnim", m_AnimAtlas, m_NewAnimLtp, m_NewAnimSlice, m_NewAnimBackSize, m_iNewAnimFrmNum, m_NewAnimFPS);
			GetTarget()->Animator2D()->SetAnim(GetTarget()->Animator2D()->FindAnim(L"NewAnim"), true);
		}
	}

	if (ImGui::Button("Return", ImVec2(60.f, 20.f)))
		m_CurAnim->SetAllFrm(m_PrevAllAnim);

	ImGui::SameLine();
	if (ImGui::Button("Load", ImVec2(60.f, 20.f)))
		Load();

	ImGui::SameLine();
	if (ImGui::Button("Save", ImVec2(60.f, 20.f)))
		Save();

	ImGui::End();
}


void Animator2DUI::ShowCurAnim()
{
	ImGui::Begin("CurAnimation", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar);
	ImVec2 CurFrmUVLtp = ImVec2(m_CurFrm.LeftTopUV.x, m_CurFrm.LeftTopUV.y);
	ImVec2 CurFrmUVSize = ImVec2(CurFrmUVLtp.x + m_CurFrm.SliceUV.x, CurFrmUVLtp.y + m_CurFrm.SliceUV.y); 
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
	ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 1.f); // 50% opaque white
	float AnimRatio = CurFrmUVSize.y / CurFrmUVSize.x;
	ImVec2 CurAnimSize = ImGui::GetWindowSize();
	ImGui::Image(m_AnimAtlas->GetSRV().Get(), CurAnimSize, CurFrmUVLtp, CurFrmUVSize, tint_col, border_col);
	ImGui::End();

	// AnimFrm 이미지 
	static bool enable_extra_decorations = false;

	ImGuiStyle& style = ImGui::GetStyle(); 
	Vec3 TargetSize = GetTarget()->Transform()->GetRelativeScale();
	ImVec2 ImageSize = ImVec2(TargetSize.x, TargetSize.y);

	float child_height = ImGui::GetTextLineHeight() + style.ScrollbarSize + style.WindowPadding.y + ImageSize.y;
	ImGuiWindowFlags child_flags = ImGuiWindowFlags_HorizontalScrollbar | (enable_extra_decorations ? ImGuiWindowFlags_AlwaysVerticalScrollbar : 0);
	ImGuiID child_id = ImGui::GetID((void*)(intptr_t)0);
	bool ShowFrmView = ImGui::BeginChild(2, ImVec2(-100, child_height), true, child_flags);
	if (ShowFrmView)
	{
		for (size_t i = 0; i < m_vecAllFrm.size(); ++i)
		{
			char szbuffer[10] = {};
			_itoa_s((int)i + 1, szbuffer, 10);
			m_mapStrData.insert(make_pair(szbuffer, false));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f,0.f,0.f,0.f));
			ImVec2 UVLtp = ImVec2(m_vecAllFrm[i].LeftTopUV.x + m_vecAllFrm[i].Offset.x, m_vecAllFrm[i].LeftTopUV.y + m_vecAllFrm[i].Offset.y);
			ImVec2 UVSize = ImVec2(UVLtp.x + m_vecAllFrm[i].SliceUV.x, UVLtp.y + m_vecAllFrm[i].SliceUV.y);
			if (ImGui::ImageButton(szbuffer, m_AnimAtlas->GetSRV().Get(), ImageSize, UVLtp, UVSize))
			{
				map<string, bool>::iterator iter = m_mapStrData.begin();
				for (; iter != m_mapStrData.end(); ++iter)
				{
					if (iter->first == szbuffer)
						iter->second = true;
					else
						iter->second = false;
				}
			}

			if (true == m_mapStrData.find(szbuffer)->second)
			{
				m_CurAnim->Stop((int)i);
				m_mapStrData.find(szbuffer)->second = false;
			}
			if (i != m_vecAllFrm.size() - 1)
				ImGui::SameLine();

			ImGui::PopStyleColor();
		}

	}
	ImGui::EndChild();

	if (ImGui::Button("AddFrm", ImVec2(60.f, 20.f)))
	{
		tAnim2DFrm NewAnimFrm = {};
		m_CurAnim->AddFrm(NewAnimFrm);
		m_CurAnim->SetCurFrmNum((int)(m_vecAllFrm.size() - 1));
	}

	ImGui::SameLine();

	ImVec2 editorpos = ImVec2(ImGui::GetWindowPos().x - m_CreateTexSize.x * 1.06f, ImGui::GetWindowPos().y);
	ImGui::SetNextWindowPos(editorpos, ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(m_CreateTexSize.x + 33, ImGui::GetWindowSize().y), ImGuiCond_Once);
	// Create 창 생성 버튼
	static int clicked = 0;
	if (ImGui::Button("EDIT", ImVec2(60.f, 20.f)))
		clicked++;
	if (clicked & 1)
	{
		ShowCreateAnimation();
	}
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "PRESS 'SPACE' to Play/Stop");
	 
	// 현재 Frm 정보
	m_CurFrm = m_CurAnim->GetCurFrame();
	Vec2 vResolution = Vec2(m_AnimAtlas->Width(), m_AnimAtlas->Height());
	int CurFrm = m_CurAnim->GetCurFrmNum();
	float fDuration = 1.f / m_vecAllFrm[CurFrm].fDuration;
	Vec2 vLeftTop = m_vecAllFrm[CurFrm].LeftTopUV * vResolution;
	Vec2 vOffset = m_vecAllFrm[CurFrm].Offset * vResolution;
	Vec2 vSlice = m_vecAllFrm[CurFrm].SliceUV * vResolution;
	ImVec2 vTexSize = m_CreateTexSize;

	int FrmNum = CurFrm + 1;
	string FrmCount = " / ";
	FrmCount += std::to_string(m_vecAllFrm.size());
	const char* szbuffer = { FrmCount.c_str()};
	ImGui::Text("FrmNum  ");
	ImGui::SameLine();
	ImGui::DragInt(szbuffer, &FrmNum);

	ImGui::Text("FPS     ");
	ImGui::SameLine(); 
	ImGui::DragFloat("##Duration", &fDuration);

	ImGui::Text("LeftTop ");
	ImGui::SameLine();
	ImGui::InputFloat2("##LeftTop", &vLeftTop.x);

	ImGui::Text("Offset  ");
	ImGui::SameLine();
	ImGui::InputFloat2("##Offset", &vOffset.x);

	ImGui::Text("Slice   ");
	ImGui::SameLine();
	ImGui::InputFloat2("##Slice", &vSlice.x);

	ImGui::Text("TexSize ");
	ImGui::SameLine();
	ImGui::InputFloat2("##AtlasTexSize", &vResolution.x);
	
	tAnim2DFrm SetFrm = {};
	SetFrm.fDuration = 1 / fDuration;
	SetFrm.LeftTopUV = vLeftTop / Vec2(m_AnimAtlas->Width(), m_AnimAtlas->Height());
	SetFrm.Offset = vOffset / Vec2(m_AnimAtlas->Width(), m_AnimAtlas->Height());
	SetFrm.SliceUV = vSlice / Vec2(m_AnimAtlas->Width(), m_AnimAtlas->Height());

	m_CurAnim->SetCurFrm(SetFrm, m_CurAnim->GetCurFrmNum());
	m_CreateTexSize = vTexSize;


	ImGui::SetNextWindowPos(editorpos, ImGuiCond_None);
}


void Animator2DUI::ShowCreateAnimation()
{
	ImGui::Begin("Animation Creator", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoDecoration);
	static bool IsMouseDraging = false;
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 ImageHoverPos = ImVec2(io.MousePos.x - pos.x, io.MousePos.y - pos.y);

	// 아틀라스 사이즈 및 비율
	ImVec2 AtlasSize = ImVec2(m_AnimAtlas->Width(), m_AnimAtlas->Height());

	// 출력하려는 크기 비율이 아틀라스와 다르다면 비율을 맞춤
	if ((m_CreateTexSize.x == m_CreateTexSize.y) && (m_CreateTexSize.y / m_CreateTexSize.x != m_AtlasRatio))
		m_CreateTexSize.y *= m_AtlasRatio;

	// 이미지 출력에 사용될 변수
	ImVec2 minuv = ImVec2(0.f, 0.f);
	ImVec2 maxuv = ImVec2(1.f, 1.f);
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
	ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

	// 이미지 배율
	static ImVec2 magni = ImVec2(1.f, 1.f);
	// 배율 사용 시 출력될 이미지 uv값 계산
	maxuv = ImVec2(maxuv.x / magni.x, maxuv.y / magni.y);

	// 실제 이미지와 출력시킬 크기 배율
	float magniX = m_CreateTexSize.x / AtlasSize.x;
	float magniY = m_CreateTexSize.y / AtlasSize.y;

	// 아틀라스 이미지 출력
	ImGui::Image(m_CurAnim->GetAtlasTex()->GetSRV().Get(), m_CreateTexSize, minuv, maxuv, tint_col, border_col);

	if (ImGui::IsItemHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{
		// 이미지에 드래그한 영역을 나타낼 사각형을 그림
		static ImVec2 RectLtp;
		static ImVec2 RextBtm;
		// 드래그 중인 게 아니라면 DragLtp을 현재 커서위치로 지정
		if (false == IsMouseDraging)
		{
			IsMouseDraging = true;
			m_DragLtp = ImageHoverPos;
			RectLtp = io.MousePos;
		}
		else if (true == IsMouseDraging)
		{
			m_DragBtm = ImageHoverPos;
			RextBtm = io.MousePos;
		}
		ImGui::GetForegroundDrawList()->AddRect(RectLtp, RextBtm, ImGui::GetColorU32(ImGuiCol_Button), 1.0f);
	}

	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		IsMouseDraging = false;
		m_CreateFrmInfo.fDuration = 0.1f;
		m_CreateFrmInfo.LeftTopUV = Vec2((m_DragLtp.x * magniX) / AtlasSize.x, (m_DragLtp.y * magniY) / AtlasSize.y);
		m_CreateFrmInfo.Offset = Vec2(0.f, 0.f);
		m_CreateFrmInfo.SliceUV = Vec2((m_DragBtm.x - m_DragLtp.x) / magniX / AtlasSize.x, (m_DragBtm.y - m_DragLtp.y) / magniY / AtlasSize.y);
		m_DragLtp = ImVec2(0.f, 0.f);
		m_DragBtm = ImVec2(0.f, 0.f);
	}

	if(ImGui::IsItemHovered())
	{
		// 드래그 중인 이미지 확대 출력
		ImGui::BeginTooltip();

		ImVec2 DragSize = ImVec2(m_DragBtm.x - m_DragLtp.x, m_DragBtm.y - m_DragLtp.y);
		float region_sz = 32.0f;
		float region_x = io.MousePos.x - pos.x;
		float region_y = io.MousePos.y - pos.y;
		float zoom = 4.0f;
		if (region_x < 0.0f) { region_x = 0.0f; }
		else if (region_x > AtlasSize.x - region_sz) { region_x = AtlasSize.x - region_sz; }
		if (region_y < 0.0f) { region_y = 0.0f; }
		else if (region_y > AtlasSize.y - region_sz) { region_y = AtlasSize.y - region_sz; }
		ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
		ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
		ImVec2 uv0 = ImVec2(region_x / AtlasSize.x, region_y / AtlasSize.y);
		ImVec2 uv1 = ImVec2((region_x + region_sz) / AtlasSize.x, (region_y + region_sz) / AtlasSize.y);
		ImGui::Image(m_CurAnim->GetAtlasTex()->GetSRV().Get(), ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
	
		ImGui::EndTooltip();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();
		draw_list->AddRect(m_DragLtp, m_DragBtm, IM_COL32(255, 255, 0, 255));
		draw_list->AddRectFilled(m_DragLtp, m_DragBtm, IM_COL32(255, 0, 255, 255));

	}

	if (ImGui::Button("Set", ImVec2(45.f, 30.f)))
	{
		m_CurAnim->SetCurFrm(m_CreateFrmInfo, m_CurAnim->GetCurFrmNum());
		m_vecAllFrm[m_CurAnim->GetCurFrmNum()] = m_CreateFrmInfo;
	}


	ImGui::End();
}

void Animator2DUI::EditCurFrm()
{
	m_CurFrm = m_CurAnim->GetCurFrame();
	if (KEY_PRESSED(KEY::LCTRL))
	{
		if (KEY_TAP(KEY::W))
			m_CurFrm.SliceUV -= Vec2(0.f, 1.f / m_AnimAtlas->Height());
		if (KEY_TAP(KEY::S))
			m_CurFrm.SliceUV += Vec2(0.f, 1.f / m_AnimAtlas->Height());
		if (KEY_TAP(KEY::A))
			m_CurFrm.SliceUV -= Vec2(1.f / m_AnimAtlas->Width(), 0.f);
		if (KEY_TAP(KEY::D))
			m_CurFrm.SliceUV += Vec2(1.f / m_AnimAtlas->Width(), 0.f);
	}
	else if(KEY_PRESSED(KEY::LSHIFT))
	{
		if (KEY_TAP(KEY::W))
			m_CurFrm.Offset -= Vec2(0.f, 1.f / m_AnimAtlas->Height());
		if (KEY_TAP(KEY::S))
			m_CurFrm.Offset += Vec2(0.f, 1.f / m_AnimAtlas->Height());
		if (KEY_TAP(KEY::A))
			m_CurFrm.Offset -= Vec2(1.f / m_AnimAtlas->Width(), 0.f);
		if (KEY_TAP(KEY::D))
			m_CurFrm.Offset += Vec2(1.f / m_AnimAtlas->Width(), 0.f);
	}
	else
	{
		if (KEY_TAP(KEY::W))
			m_CurFrm.LeftTopUV -= Vec2(0.f, 1.f / m_AnimAtlas->Height());
		if (KEY_TAP(KEY::S))
			m_CurFrm.LeftTopUV += Vec2(0.f, 1.f / m_AnimAtlas->Height());
		if (KEY_TAP(KEY::A))
			m_CurFrm.LeftTopUV -= Vec2(1.f / m_AnimAtlas->Width(), 0.f);
		if (KEY_TAP(KEY::D))
			m_CurFrm.LeftTopUV += Vec2(1.f / m_AnimAtlas->Width(), 0.f);
	}

	if (KEY_TAP(KEY::Q))
	{
		float FPS = 1.f / m_CurFrm.fDuration;
		FPS -= 1;
		m_CurFrm.fDuration = 1.f / FPS;
	}

	if (KEY_TAP(KEY::E))
	{
		float FPS = 1.f / m_CurFrm.fDuration;
		FPS += 1;
		m_CurFrm.fDuration = 1.f / FPS;
	}
	if (KEY_TAP(KEY::Z))
	{
		m_CurAnim->SetCurFrmNum(0);
	}
	if (KEY_TAP(KEY::X))
	{
		int FrmNum = m_CurAnim->GetCurFrmNum();
		if (0 < FrmNum)
			FrmNum -= 1;
		m_CurAnim->SetCurFrmNum(FrmNum);
	}
	if (KEY_TAP(KEY::C))
	{
		int FrmNum = m_CurAnim->GetCurFrmNum();
		if ((int)m_vecAllFrm.size() - 1 > FrmNum)
			FrmNum += 1;
		m_CurAnim->SetCurFrmNum(FrmNum);
	}
	if (KEY_TAP(KEY::V))
	{
		m_CurAnim->SetCurFrmNum((int)m_vecAllFrm.size() - 1);
	}

	m_CurAnim->SetCurFrm(m_CurFrm, m_CurAnim->GetCurFrmNum());
}

void Animator2DUI::SelectAnim(DWORD_PTR _Key)
{
	// _key로 선택된 Mesh를 찾아내 Target의 Mesh로 설정함
	string strKey = (char*)_Key;
	wstring animname = wstring(strKey.begin(), strKey.end());
	GetTarget()->Animator2D()->Play(animname, true);
	vector<tAnim2DFrm> ChangeAnimFrm =  GetTarget()->Animator2D()->FindAnim(animname)->GetAllFrm();
}

void Animator2DUI::SelectAtlas(DWORD_PTR _Key)
{
	// _key로 선택된 Mesh를 찾아내 Target의 Mesh로 설정함
	string strKey = (char*)_Key;
	Ptr<CTexture>SeletedTex = CResMgr::GetInst()->FindRes<CTexture>(wstring(strKey.begin(), strKey.end()));
	GetTarget()->Animator2D()->GetCurAnim()->SetAtlasTex(SeletedTex);
	m_AnimAtlas = SeletedTex;
	m_AtlasRatio = m_AnimAtlas->Width() / m_AnimAtlas->Height();
	m_CreateTexSize = ImVec2(500.f, 500.f);
}

void Animator2DUI::Save()
{
	OPENFILENAME ofn = {};

	wstring strTileFolderPath = CPathMgr::GetInst()->GetContentPath();
	strTileFolderPath += L"animation\\";

	wchar_t szFilePath[256] = {};

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	ofn.lpstrFilter = L".anim\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = szFilePath;
	ofn.nMaxFileTitle = 64;
	ofn.lpstrInitialDir = strTileFolderPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetSaveFileName(&ofn))
		return;

	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, szFilePath, L"wb");

	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Animation Save 실패, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"파일 저장 실패", MB_OK);
		return;
	}

	// Animation 이름 저장
	fwprintf_s(pFile, L"\n");

	fwprintf_s(pFile, L"[ANIMATION_NAME]\n");
	fwprintf_s(pFile, szFilePath);
	fwprintf_s(pFile, L"\n\n");

	// 아틀라스 텍스쳐 키값 저장	
	fwprintf_s(pFile, L"[ATLAS_KEY]\n");
	fwprintf_s(pFile, m_CurAnim->GetAtlasTex()->GetKey().c_str());
	fwprintf_s(pFile, L"\n\n");

	fwprintf_s(pFile, L"[BACK_SIZE]\n");
	fwprintf_s(pFile, L"%.1f %.1f\n", m_NewAnimBackSize.x , m_NewAnimBackSize.y);
	fwprintf_s(pFile, L"\n\n");

	// 프레임 정보 저장
	wchar_t szNum[50] = {};

	fwprintf_s(pFile, L"[FRAME_COUNT]\n");
	size_t iFrmCount = m_vecAllFrm.size();
	_itow_s((int)iFrmCount, szNum, 50, 10);

	fwprintf_s(pFile, szNum);
	fwprintf_s(pFile, L"\n\n");

	for (size_t i = 0; i < iFrmCount; ++i)
	{
		fwprintf_s(pFile, L"[%zd_FRAME]\n", i);

		// LEFT_TOP
		fwprintf_s(pFile, L"[LEFT_TOP]\n");
		fwprintf_s(pFile, L"%.5f %.5f\n", m_vecAllFrm[i].LeftTopUV.x, m_vecAllFrm[i].LeftTopUV.y);

		// SIZE
		fwprintf_s(pFile, L"[SIZE]\n");
		fwprintf_s(pFile, L"%.5f %.5f\n", m_vecAllFrm[i].SliceUV.x, m_vecAllFrm[i].SliceUV.y);

		// OFFSET
		fwprintf_s(pFile, L"[OFFSET]\n");
		fwprintf_s(pFile, L"%.5f %.5f\n", m_vecAllFrm[i].Offset.x, m_vecAllFrm[i].Offset.y);

		// DURATION
		fwprintf_s(pFile, L"[DURATION]\n");
		fwprintf_s(pFile, L"%.3f\n", m_vecAllFrm[i].fDuration);

		fwprintf_s(pFile, L"\n");
	}

	fclose(pFile);
}

void Animator2DUI::Load()
{
	OPENFILENAME ofn = {};

	wstring strTileFolderPath = CPathMgr::GetInst()->GetContentPath();
	strTileFolderPath += L"animation\\";

	wchar_t szFilePath[256] = {};

	// 파일탐색창을 만드는 코드
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFilePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 256;
	// 취급하는 파일 타입
	ofn.lpstrFilter = L".anim\*";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = strTileFolderPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (false == GetOpenFileName(&ofn))
		return;

	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, szFilePath, L"rb");


	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Animation Save 실패, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"파일 저장 실패", MB_OK);
		return;
	}

	size_t iFrameCount = 0;
	wstring strAnimName;
	wstring strAtlasKey;
	Vec2 BackSize;

	while (true)
	{
		wchar_t szBuffer[256] = {};
		fwscanf_s(pFile, L"%s", szBuffer, 256);

		if (!wcscmp(szBuffer, L"[ANIMATION_NAME]"))
		{
			fwscanf_s(pFile, L"%s", szBuffer, 256);
			strAnimName = szBuffer;
		}

		else if (!wcscmp(szBuffer, L"[ATLAS_KEY]"))
		{
			fwscanf_s(pFile, L"%s", szBuffer, 256);
			strAtlasKey = szBuffer;
		}

		else if (!wcscmp(szBuffer, L"[BACK_SIZE]"))
		{
			fwscanf_s(pFile, L"%f %f", &BackSize.x, &BackSize.y);
		}

		else if (!wcscmp(szBuffer, L"[FRAME_COUNT]"))
		{
			fwscanf_s(pFile, L"%zd", &iFrameCount);
			Ptr<CTexture> Atlas = CResMgr::GetInst()->FindRes<CTexture>(strAtlasKey);
			GetTarget()->Animator2D()->CreateAnimation(strAnimName, Atlas, Vec2(0.f, 0.f), Vec2(0.f, 0.f), BackSize, (int)iFrameCount, 0);
			for (size_t i = 0; i < iFrameCount; ++i)
			{
				tAnim2DFrm frm = {};

				while (true)
				{
					wchar_t szBuffer[256] = {};
					fwscanf_s(pFile, L"%s", szBuffer, 256);

					if (!wcscmp(szBuffer, L"[LEFT_TOP]"))
					{
						fwscanf_s(pFile, L"%f %f", &frm.LeftTopUV.x, &frm.LeftTopUV.y);
					}

					else if (!wcscmp(szBuffer, L"[SIZE]"))
					{
						fwscanf_s(pFile, L"%f %f", &frm.SliceUV.x, &frm.SliceUV.y);
					}

					else if (!wcscmp(szBuffer, L"[OFFSET]"))
					{
						fwscanf_s(pFile, L"%f %f", &frm.Offset.x, &frm.Offset.y);
					}

					else if (!wcscmp(szBuffer, L"[DURATION]"))
					{
						fwscanf_s(pFile, L"%f", &frm.fDuration);
						break;
					}
				}

				GetTarget()->Animator2D()->FindAnim(strAnimName)->SetCurFrm(frm, (int)i);
			}

			break;
		}
	}
	GetTarget()->Animator2D()->SetAnim(GetTarget()->Animator2D()->FindAnim(strAnimName), true);
	m_PrevAllAnim = GetTarget()->Animator2D()->GetCurAnim()->GetAllFrm();
}
