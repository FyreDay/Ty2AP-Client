#include "pch.h"
#include "CheckHandler.h"
#include "gui.h"
#include "InfoWindow.h"

typedef void(__stdcall* FunctionType)();

//0011b2ba hook
//0011b2ba + 5 return
FunctionType CollectCollectibleOrigin = nullptr;
uintptr_t CollectCollectibleOriginReturnAddr;
FunctionType CompleteMissionOrigin = nullptr;
uintptr_t CompleteMissionOriginReturnAddr;

__declspec(naked) void __stdcall CheckHandler::CollectCollectibleHook() {
	_asm {
		push ebx
		push edx
		push edi
		push eax
		push ecx
		push esi
		call CheckHandler::OnCollectCollectible
		pop esi
		pop ecx
		pop eax
		pop edi
		pop edx
		pop ebx
		lea edi, [edx + eax * 0x4]
		mov eax, ecx
		jmp dword ptr[CollectCollectibleOriginReturnAddr]
	}
}

__declspec(naked) void __stdcall CheckHandler::CompleteMissionHook() {
	_asm {
		push ebx
		push edi
		push eax
		push esi
		push edx
		push ecx
		call CheckHandler::OnCompleteMission
		pop ecx
		pop edx
		pop esi
		pop eax
		pop edi
		pop ebx
		add edx,eax
		cmp ecx, dword ptr [esi + 0x4]
		jmp dword ptr[CompleteMissionOriginReturnAddr]
	}
}

void CheckHandler::SetupHooks() {
	CollectCollectibleOriginReturnAddr = Core::moduleBase + 0x0011b2ba + 5;
	auto ccaddr = (char*)(Core::moduleBase + 0x0011b2ba);
	MH_CreateHook((LPVOID)ccaddr, &CollectCollectibleHook, reinterpret_cast<LPVOID*>(&CollectCollectibleOrigin));

	CompleteMissionOriginReturnAddr = Core::moduleBase + 0x00297374 + 5;
	auto cmaddr = (char*)(Core::moduleBase + 0x00297374);
	MH_CreateHook((LPVOID)cmaddr, &CompleteMissionHook, reinterpret_cast<LPVOID*>(&CompleteMissionOrigin));
}

void CheckHandler::OnCollectCollectible(int type, int id) {
	for (auto& window : GUI::windows) {
		if (auto infowindow = dynamic_cast<InfoWindow*>(window.get())) {
			infowindow->AddLogMessage(collectibles[type].name + " " +  std::to_string(id));
		}
	}
	API::LogPluginMessage("maybe id: " + std::to_string(id) + " maybe type: " + std::to_string(type));
}

void CheckHandler::OnCompleteMission(void* mission) {
	uint8_t* base = static_cast<uint8_t*>(mission);
	short status = *reinterpret_cast<short*>(base + 0x10); // read status
	if (status < 5) {
		return;
	}
	short value = *reinterpret_cast<short*>(base + 0x4); // read short
	char letter = *reinterpret_cast<char*>(base + 0x7);  // read char
	std::string id = std::string(1, letter) + std::to_string(value);
	for (auto& window : GUI::windows) {
		if (auto infowindow = dynamic_cast<InfoWindow*>(window.get())) {
			infowindow->AddLogMessage(id + " -> " + std::to_string(status));
		}
	}
}


