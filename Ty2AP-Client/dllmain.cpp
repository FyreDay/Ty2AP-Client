#include "pch.h"
#include "GameHandler.h"
#include "gui.h"

bool disabledButtons = false;

void TickBeforeGame(float deltaSeconds) {
    
    GUI::DrawUI();
    if (!disabledButtons && MKUI::GetMainMenu() != nullptr) {
        GameHandler::DisableLoadButtons();
        disabledButtons = true;
    }

    if (GameHandler::g_SaveCallback.active) {
        if (--GameHandler::g_SaveCallback.framesRemaining <= 0) {
            if (GameHandler::g_SaveCallback.callback != nullptr) {
                GameHandler::g_SaveCallback.callback();
                GameHandler::g_SaveCallback.callback = nullptr;
            }
            else {
                *(DWORD*)(GameHandler::g_SaveCallback.esi + 0x238) = 0;  // Mark save complete
            }
            GameHandler::g_SaveCallback.active = false;
            
            
        }
    }

    
}

void OnTyInit() {
    GameHandler::Initialize();
    std::thread t(ArchipelagoHandler::Poll);
    t.detach();
}

void OnTyBeginShutdown() {
    ArchipelagoHandler::DisconnectAP();
}

extern "C" __declspec(dllexport) bool TygerFrameworkPluginInitialize(TygerFrameworkPluginInitializeParam* param) {
    //Make sure to call this first before any API Functions
    if (!API::Initialize(param))
        return false;

    if (!Core::initialize((HMODULE)API::Get()->param()->TyHModule))
        return false;

    API::AddTickBeforeGame(TickBeforeGame);
    API::AddPluginImGuiWantCaptureMouse((ImGuiWantCaptureMouseFunc)GUI::ImGuiWantCaptureMouse);
    API::AddOnTyInitialized(OnTyInit);
    API::AddPluginWndProc((WndProcFunc)GUI::WndProc);
    return true;
}

extern "C" __declspec(dllexport) void TygerFrameworkPluginRequiredVersion(TygerFrameworkPluginVersion* version) {
    version->Major = 1;
    version->Minor = 1;
    version->Patch = 3;
    version->CompatibleGames = { 2 };
}