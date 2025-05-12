#include "pch.h"

void TickBeforeGame(float deltaSeconds) {
}

bool WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_KEYDOWN && wParam == VK_F3) {
        
        int id = 3680;
               
        uintptr_t result = MKObject::GetMKObject(id);
        API::LogPluginMessage(std::to_string(result));
        
    }
    return false;
}


extern "C" __declspec(dllexport) bool TygerFrameworkPluginInitialize(TygerFrameworkPluginInitializeParam* param) {
    //Make sure to call this first before any API Functions
    if (!API::Initialize(param))
        return false;

    if (!Core::initialize((HMODULE)API::Get()->param()->TyHModule))
        return false;

    API::AddTickBeforeGame(TickBeforeGame);
    API::AddPluginWndProc((WndProcFunc)WndProc);
    return true;
}

extern "C" __declspec(dllexport) void TygerFrameworkPluginRequiredVersion(TygerFrameworkPluginVersion* version) {
    version->Major = 1;
    version->Minor = 1;
    version->Patch = 3;
    version->CompatibleGames = { 2 };
}