#include "pch.h"
#include "gui.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "RobotoMedium.hpp"

std::vector<std::unique_ptr<Window>> GUI::windows;
std::map<std::string, unsigned int> GUI::icons;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool GUI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_KEYDOWN && wParam == 0x54) {
        for (auto& window : windows) {
            if (auto Lwindow = dynamic_cast<TrackerWindow*>(window.get())) {
                Lwindow->visibleOverride = true;
                break;
            }
        }
    }
    if (msg == WM_KEYUP && wParam == 0x54) {
        for (auto& window : windows) {
            if (auto Lwindow = dynamic_cast<TrackerWindow*>(window.get())) {
                Lwindow->visibleOverride = false;
                break;
            }
        }
    }
    if (API::DrawingGUI())
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;
    return false;
}

void GUI::Initialize() {
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    fontRegular = io.Fonts->AddFontFromMemoryCompressedTTF(RobotoMedium_compressed_data, RobotoMedium_compressed_size, 16);
	fontLarge = io.Fonts->AddFontFromMemoryCompressedTTF(RobotoMedium_compressed_data, RobotoMedium_compressed_size, 24);
	fontSmall = io.Fonts->AddFontFromMemoryCompressedTTF(RobotoMedium_compressed_data, RobotoMedium_compressed_size, 12);
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

    if (!GUI::LoadIcons()) {
        API::LogPluginMessage("Failed to load icons.");
        return;
    }

    windows.push_back(std::make_unique<LoginWindow>());
    windows.push_back(std::make_unique<LoggerWindow>());
    windows.push_back(std::make_unique<InfoWindow>());
    windows.push_back(std::make_unique<TrackerWindow>());

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
    ImGui::SetNextWindowPos(ImVec2(windowWidth - (114 * uiScale), 0));
    ImGui::SetNextWindowSize(ImVec2(114 * uiScale, 114 * uiScale));
    auto popupId = "APMenu";
    ImGui::Begin("AP Button", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
    if (ImGui::ImageButton("##AP_BUTTON", (ImTextureID)(intptr_t)GUI::icons["ap"], ImVec2(64 * uiScale, 64 * uiScale), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1))) {
        ImGui::OpenPopup(popupId);
    }
    ImGui::PopStyleColor(3);

    if (ImGui::BeginPopup(popupId)) {
        if (ImGui::MenuItem("Connection")) {
            for (auto& window : windows) {
                if (auto login = dynamic_cast<LoginWindow*>(window.get())) {
                    login->isVisible = true;
                    break;
                }
            }
        }
        if (ImGui::MenuItem("Collectible Info")) {
            for (auto& window : windows) {
                if (auto info = dynamic_cast<InfoWindow*>(window.get())) {
                    info->isVisible = true;
                    break;
                }
            }
        }
        if (ImGui::MenuItem("Tracker Window")) {
            for (auto& window : windows) {
                if (auto info = dynamic_cast<TrackerWindow*>(window.get())) {
                    info->ToggleVisibility();
                    break;
                }
            }
        }
        if (ImGui::MenuItem("Filter Log")) {
            GUI::filterToSelf = !GUI::filterToSelf;
        }
        ImGui::EndPopup();
    }
    ImGui::End();

    for (auto& window : windows) { window.get()->Draw(windowWidth, windowHeight, uiScale); }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool GUI::ImGuiWantCaptureMouse() {
    return GUI::init && ImGui::GetIO().WantCaptureMouse;
}

GLuint GUI::LoadTextureFromResource(int resource_id) {
    auto hModule = GetModuleHandle(std::wstring(API::PluginName.begin(), API::PluginName.end()).c_str());

    HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(resource_id), L"PNG");
    if (!hResource) {
        API::LogPluginMessage("Failed to find resource\n");
        return 0;
    }

    HGLOBAL hMemory = LoadResource(hModule, hResource);
    if (!hMemory) {
        API::LogPluginMessage("Failed to load resource\n");
        return 0;
    }

    DWORD size = SizeofResource(hModule, hResource);
    if (size == 0) {
        API::LogPluginMessage("Resource size is zero\n");
        return 0;
    }

    void* pData = LockResource(hMemory);
    if (!pData) {
        API::LogPluginMessage("Failed to lock resource\n");
        return 0;
    }

    int width, height, channels;
    unsigned char* data = stbi_load_from_memory((unsigned char*)pData, size, &width, &height, &channels, 4);
    if (!data) {
        API::LogPluginMessage(stbi_failure_reason());
        return 0;
    }

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    return texture_id;
}

bool GUI::LoadIcons() {
    icons["mission"] = GUI::LoadTextureFromResource(MISSION_ICON);

    icons["battlekey"] = GUI::LoadTextureFromResource(BATTLE_BUNYIP_ICON);
    icons["lifterkey"] = GUI::LoadTextureFromResource(LIFTER_BUNYIP_ICON);
    icons["thermokey"] = GUI::LoadTextureFromResource(THERMO_BUNYIP_ICON);
    icons["subkey"] = GUI::LoadTextureFromResource(SUB_BUNYIP_ICON);

    icons["bilbymap"] = GUI::LoadTextureFromResource(BILBY_MAP_ICON);
    icons["cogmap"] = GUI::LoadTextureFromResource(COG_MAP_ICON);
    icons["stevemap"] = GUI::LoadTextureFromResource(STEVE_MAP_ICON);

    icons["barrier_p"] = GUI::LoadTextureFromResource(BARRIER_P_ICON);
    icons["barrier_b"] = GUI::LoadTextureFromResource(BARRIER_B_ICON);
    icons["barrier_f"] = GUI::LoadTextureFromResource(BARRIER_F_ICON);

	icons["orb"] = GUI::LoadTextureFromResource(ORB_ICON);
    icons["cog"] = GUI::LoadTextureFromResource(COG_ICON);

    icons["ap"] = GUI::LoadTextureFromResource(AP_ICON);
    return true;
}