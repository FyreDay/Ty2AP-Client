#include "pch.h"

void TickBeforeGame(float deltaSeconds) {
}

bool WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_KEYDOWN && wParam == VK_F2) {
        uintptr_t result;
        int id = 3680;
        uintptr_t(*getObjPtr)(int) = reinterpret_cast<uintptr_t(*)(int)>(((uintptr_t)API::Get()->param()->TyHModule) + 0x00314f10);
        __asm{
            push id
            call getObjPtr
            add esp, 0x4
            mov result, eax
        }
        API::LogPluginMessage(std::to_string(result));
        
    }
    return false;
}


extern "C" __declspec(dllexport) bool TygerFrameworkPluginInitialize(TygerFrameworkPluginInitializeParam* param) {
    //Make sure to call this first before any API Functions
    if (!API::Initialize(param))
        return false;

    API::AddTickBeforeGame(TickBeforeGame);
    API::AddPluginWndProc((WndProcFunc)WndProc);
    API::LogPluginMessage(std::to_string(((int)API::Get()->param()->TyHModule)));
    return true;
}

extern "C" __declspec(dllexport) void TygerFrameworkPluginRequiredVersion(TygerFrameworkPluginVersion* version) {
    version->Major = 1;
    version->Minor = 1;
    version->Patch = 3;
    version->CompatibleGames = { 2 };
}