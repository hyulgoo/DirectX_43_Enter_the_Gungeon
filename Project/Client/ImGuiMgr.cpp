#include "pch.h"
#include "ImGuiMgr.h"

#include <Engine/CDevice.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CPathMgr.h>

#include "UI.h"
#include "ParamUI.h"
#include <fstream>
#include <Engine/CTransform.h>


ImGuiMgr::ImGuiMgr()
    : m_hMainHwnd(nullptr)
    , m_bOverlay(true)
    , m_hObserver(nullptr)
{
}

ImGuiMgr::~ImGuiMgr()
{
    // ImGui Release
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    Safe_Del_Map(m_mapUI);

}

void ImGuiMgr::init(HWND _hWnd)
{
    m_hMainHwnd = _hWnd;

    // ImGui 초기화
   // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        style.WindowRounding = 9;
        style.ChildRounding = 12;
        style.TabRounding = 5;
        style.FrameRounding = 5;
        style.GrabRounding = 5;
        style.ScrollbarSize = 12;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(m_hMainHwnd);
    ImGui_ImplDX11_Init(DEVICE, CONTEXT);

    LoadFont();

    CreateUI();
}

void ImGuiMgr::progress()
{
    begin();

    tick();

    finaltick();

    render();

    ObserveContent();
}


void ImGuiMgr::begin()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ParamUI::g_NextId = 0;
}

void ImGuiMgr::tick()
{
    for (const auto& pair : m_mapUI)
    {
        pair.second->tick();
    }
}

void ImGuiMgr::finaltick()
{
    // 데모 UI
    ImGui::ShowDemoWindow();

    ShowOverlay(&m_bOverlay);
    
    // InspectorUI final tick
    for (const auto& pair : m_mapUI)
    {
        if (pair.second->IsActive())
        {
            pair.second->finaltick();
        }
    }

    // Player 이동 및 조작 때 UI가 응답하지 않게
    if (KEY_TAP(KEY::ENTER))
        ImGui::SetWindowFocus(nullptr);
}

void ImGuiMgr::render()
{
    // ImGui Rendering
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void ImGuiMgr::LoadFont()
{
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = nullptr;
    wstring strTileFolderPath = CPathMgr::GetInst()->GetContentPath();
    strTileFolderPath += L"font/consola.ttf";
    std::string font_file = {};
    font_file = string(strTileFolderPath.begin(), strTileFolderPath.end());

    std::ifstream ifile;
    ifile.open(font_file);
    if (ifile) {
        font = io.Fonts->AddFontFromFileTTF(font_file.c_str()
        , 16.0f, NULL, io.Fonts->GetGlyphRangesKorean());
    }
    else {

        font = io.Fonts->AddFontDefault();
    }
    IM_ASSERT(font != NULL);
}

#include "InspectorUI.h"
#include "ListUI.h"
#include "ContentUI.h"
#include "OutlinerUI.h"
#include "MenuUI.h"

void ImGuiMgr::CreateUI()
{
    UI* pUI = nullptr;

    // InspectorUI
    pUI = new InspectorUI;
    pUI->SetActive(true);
    m_mapUI.insert(make_pair(pUI->GetID(), pUI));

    // ContentUI
    pUI = new ContentUI;
    pUI->SetActive(true);
    m_mapUI.insert(make_pair(pUI->GetID(), pUI));

    // Outliner
    pUI = new OutlinerUI;
    pUI->SetActive(true);
    m_mapUI.insert(make_pair(pUI->GetID(), pUI));
    
    pUI = new MenuUI;
    pUI->SetActive(true);
    m_mapUI.insert(make_pair(pUI->GetID(), pUI));

    // ListUI
    pUI = new ListUI;
    pUI->SetModal(true);
    pUI->SetActive(false);
    m_mapUI.insert(make_pair(pUI->GetID(), pUI));

    for (const auto& pair : m_mapUI)
    {
        pair.second->init();
    }
}

void ImGuiMgr::ObserveContent()
{
    // 변화를 감시할 핸들과 감시까지 대기할 시간.
    // 현재 싱글스레드로 하고 있기 때문에 대기시간을 주면 엔진이 멈춰버림
    // 따라서 변화를 기다리지 않고 매 틱마다 확인하도록 함.
    DWORD dwWaitStatus = WaitForSingleObject(m_hObserver, 0);

    if (dwWaitStatus == WAIT_OBJECT_0)
    {
        // content 폴더에 변경점이 생겼을 때
        ContentUI* UI = (ContentUI*)FindUI("##Content");
        UI->Reload();

        // 변경점을 찾는 순간 감시가 해제되므로 다음 변경도 감지하도록 지정
        FindNextChangeNotification(m_hObserver);
    }
}

UI* ImGuiMgr::FindUI(const string& _UIName)
{
    map<string, UI*>::iterator iter = m_mapUI.find(_UIName);

    if (iter == m_mapUI.end())
        return nullptr;

    return iter->second;
}
#include <Engine/CRenderMgr.h>
void ImGuiMgr::ShowOverlay(bool* _bShow)
{
    static int location = 0;
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (location >= 0)
    {
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    else if (location == -2)
    {
        // Center window
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    Vec3 Ray = CRenderMgr::GetInst()->GetRay();
    if (ImGui::Begin("overlay", _bShow, window_flags))
    {
        if (ImGui::IsMousePosValid())
            ImGui::Text("Cursor Pos: (%.f,%.f)", Ray.x , Ray.y);
        else
            ImGui::Text("Cursor Pos: <invalid>");
    }
    ImGui::End();
}
