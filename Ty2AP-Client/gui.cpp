#include "pch.h"
#include "gui.h"



std::vector<std::unique_ptr<Window>> GUI::windows;
//std::map<std::string, unsigned int> GUI::icons;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool GUI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_KEYDOWN && wParam == VK_F3) {
        for (auto& window : windows) {
            if (auto Iwindow = dynamic_cast<InfoWindow*>(window.get())) {
                Iwindow->ToggleVisibility();
                API::LogPluginMessage("Toggle InfoWindow window.");
                break;
            }
        }
    }
    if (msg == WM_KEYDOWN && wParam == VK_F2) {
        for (auto& window : windows) {
            if (auto Lwindow = dynamic_cast<LoginWindow*>(window.get())) {
                Lwindow->ToggleVisibility();
                API::LogPluginMessage("Toggle LoginWindow window.");
                break;
            }
        }
    }

    //if (msg == WM_KEYDOWN && wParam == VK_F3) {
    //    for (auto& window : windows) {
    //        if (auto Cwindow = dynamic_cast<LoggerWindow*>(window.get())) {
    //            Cwindow->ToggleVisibility();
    //            API::LogPluginMessage("Toggle LoggerWindow window.");
    //            break;
    //        }
    //    }
    //}

    if (API::DrawingGUI())
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;
    return false;
}

ImFont* GUI::tyFont = nullptr;

void GUI::Initialize() {
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    //load font
    //HMODULE hModule = nullptr;
    //if (!GetModuleHandleEx(
    //    GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
    //    GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
    //    (LPCWSTR)&GUI::Initialize,  // or any function in your DLL
    //    &hModule))
    //{
    //    API::LogPluginMessage("Failed to get module handle");
    //    return;
    //}
    //HRSRC hRes = FindResource(hModule, MAKEINTRESOURCE(IDR_RCDATA1), RT_RCDATA);
    //if (hRes) {
    //    API::LogPluginMessage("hres success");
    //    HGLOBAL hData = LoadResource(NULL, hRes);
    //    if (hData) {
    //        API::LogPluginMessage("hdata success");
    //        void* pFontData = LockResource(hData);
    //        DWORD size = SizeofResource(NULL, hRes);

    //        tyFont = io.Fonts->AddFontFromMemoryTTF(pFontData, size, 16.0f);
    //        if (!tyFont) {
    //            API::LogPluginMessage("Failed to load embedded font.");
    //        }
    //        else {
    //            API::LogPluginMessage("Found Font.");
    //        }
    //        io.Fonts->Build();
    //    }
    //}
    API::LogPluginMessage("hres done");

    io.FontGlobalScale = 1.3f;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    HWND tyWindowHandle = API::GetTyWindowHandle();
    if (tyWindowHandle == 0) {
        API::LogPluginMessage("Error: Invalid window handle.");
        GUI::init = false;
        return;
    }

    ImGui_ImplOpenGL3_Init();
    ImGui_ImplWin32_InitForOpenGL(API::GetTyWindowHandle());

    windows.push_back(std::make_unique<InfoWindow>());
    windows.push_back(std::make_unique<LoginWindow>());
    windows.push_back(std::make_unique<LoggerWindow>());

    API::LogPluginMessage("Initialized ImGui successfully.");
    GUI::init = true;
}

void GUI::DrawUI() {
    if (!GUI::init)
        Initialize();

    HWND hwnd = (HWND)API::GetTyWindowHandle();
    RECT rect;
    if (!GetClientRect(hwnd, &rect)) {
        API::LogPluginMessage("Failed to get window size.");
        return;
    }
    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;
    const int baseWidth = 1920;
    const int baseHeight = 1080;
    float widthScale = static_cast<float>(windowWidth) / baseWidth;
    float heightScale = static_cast<float>(windowHeight) / baseHeight;
    float uiScale = (((widthScale) < (heightScale)) ? (widthScale) : (heightScale));

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    //ImGui::SetNextWindowPos(ImVec2(windowWidth - (114 * uiScale), 0));



    for (auto& window : windows) { window.get()->Draw(windowWidth, windowHeight, uiScale, tyFont); }


    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool GUI::ImGuiWantCaptureMouse() {
    return GUI::init && ImGui::GetIO().WantCaptureMouse;
}
