#include "pch.h"
#include "GameHandler.h"

void GameHandler::Initialize()
{
	MH_Uninitialize();
	MH_Initialize();

	CheckHandler::SetupHooks();
	SetupHooks();
	MH_EnableHook(MH_ALL_HOOKS);
}

typedef void(__stdcall* FunctionType)();
FunctionType ItemAvailableUIOrigin = nullptr;
uintptr_t ItemAvailableUIBranchReturnAddr;
uintptr_t ItemAvailableUIOriginReturnAddr;


__declspec(naked) void __stdcall GameHandler::ItemAvailableUIHook() {
	_asm {
		push ebx
		push edx
		push edi
		push ecx
		push esi
		mov edx, eax
		push eax
		call GameHandler::OnItemAvailable
		test al, al
		jne not_equal
		pop eax
		pop esi
		pop ecx
		pop edi
		pop edx
		pop ebx
		push 0x3c5
		jmp dword ptr[ItemAvailableUIOriginReturnAddr]
		
		not_equal:
		pop eax
		pop esi
		pop ecx
		pop edi
		pop edx
		pop ebx
		jmp dword ptr[ItemAvailableUIBranchReturnAddr]
	}
}

FunctionType ItemAvailableLogicOrigin = nullptr;
uintptr_t ItemAvailableLogicBranchReturnAddr;
uintptr_t ItemAvailableLogicOriginReturnAddr;
__declspec(naked) void __stdcall GameHandler::ItemAvailableLogicHook() {
	_asm {
		push ebx
		push edx
		push edi
		push ecx
		push esi
		mov edx, eax
		push eax
		call GameHandler::OnItemAvailable
		test al, al
		je equal
		pop eax
		pop esi
		pop ecx
		pop edi
		pop edx
		pop ebx
		jmp dword ptr[ItemAvailableLogicOriginReturnAddr]

		equal:
		pop eax
			pop esi
			pop ecx
			pop edi
			pop edx
			pop ebx
			jmp dword ptr[ItemAvailableLogicBranchReturnAddr]
	}
}
FunctionType ItemAvailableClickOrigin = nullptr;
uintptr_t ItemAvailableClickBranchReturnAddr;
uintptr_t ItemAvailableClickOriginReturnAddr;
__declspec(naked) void __stdcall GameHandler::ItemAvailableClickHook() {
	_asm {
		push ebx
		push edx
		push edi
		push ecx
		push esi
		mov edx, eax
		push eax
		call GameHandler::OnItemAvailable
		test al, al
		je equal
		pop eax
		pop esi
		pop ecx
		pop edi
		pop edx
		pop ebx
		jmp dword ptr[ItemAvailableClickOriginReturnAddr]
		
		equal:
		pop eax
		pop esi
		pop ecx
		pop edi
		pop edx
		pop ebx
		jmp dword ptr[ItemAvailableClickBranchReturnAddr]
	}
}
FunctionType ItemAvailableClick2Origin = nullptr;
uintptr_t ItemAvailableClick2BranchReturnAddr;
uintptr_t ItemAvailableClick2OriginReturnAddr;
__declspec(naked) void __stdcall GameHandler::ItemAvailableClick2Hook() {
	_asm {
		push ebx
		push edx
		push edi
		push ecx
		push esi
		mov edx, eax
		push eax
		call GameHandler::OnItemAvailable
		test al, al
		je equal
		pop eax
		pop esi
		pop ecx
		pop edi
		pop edx
		pop ebx
		jmp dword ptr[ItemAvailableClick2OriginReturnAddr]
		
		equal:
		pop eax
		pop esi
		pop ecx
		pop edi
		pop edx
		pop ebx
		jmp dword ptr[ItemAvailableClick2BranchReturnAddr]
	}
}
FunctionType ChunkLoadedOrigin = nullptr;
uintptr_t ChunkLoadedOriginReturnAddr;
__declspec(naked) void __stdcall GameHandler::ChunkLoadedHook() {
	_asm {
		pushad
		call GameHandler::OnChunkLoaded
		popad
		pop esi
		pop edi
		mov esp,ebp
		pop ebp
		jmp dword ptr[ChunkLoadedOriginReturnAddr]
	}
}
FunctionType TriggerHitOrigin = nullptr;
uintptr_t TriggerHitOriginReturnAddr;
__declspec(naked) void __stdcall GameHandler::TriggerHitHook() {
	_asm {
		pushad
		call GameHandler::TryDisableFourbieTrigger
		popad
		jmp dword ptr[TriggerHitOriginReturnAddr]
	}
}


typedef int(__cdecl* TGetStringFunc)(int param_1);
TGetStringFunc originalGetString = nullptr;

void GameHandler::SetupHooks() {
	ItemAvailableUIBranchReturnAddr = Core::moduleBase + 0x1b2cba;
	ItemAvailableUIOriginReturnAddr = Core::moduleBase + 0x1b2cb0;
	
	auto iauaddr = (char*)(Core::moduleBase + 0x1b2ca5);
	MH_CreateHook((LPVOID)iauaddr, &ItemAvailableUIHook, reinterpret_cast<LPVOID*>(&ItemAvailableUIOrigin));

	ItemAvailableLogicBranchReturnAddr = Core::moduleBase + 0x1b2c47;
	ItemAvailableLogicOriginReturnAddr = Core::moduleBase + 0x1b2c31 + 6;

	auto ialaddr = (char*)(Core::moduleBase + 0x1b2c31);
	MH_CreateHook((LPVOID)ialaddr, &ItemAvailableLogicHook, reinterpret_cast<LPVOID*>(&ItemAvailableLogicOrigin));

	ItemAvailableClickBranchReturnAddr = Core::moduleBase + 0x1b111e;
	ItemAvailableClickOriginReturnAddr = Core::moduleBase + 0x1b110f + 6;

	auto iacaddr = (char*)(Core::moduleBase + 0x1b110f);
	MH_CreateHook((LPVOID)iacaddr, &ItemAvailableClickHook, reinterpret_cast<LPVOID*>(&ItemAvailableClickOrigin));

	ItemAvailableClick2BranchReturnAddr = Core::moduleBase + 0x1b10e1;
	ItemAvailableClick2OriginReturnAddr = Core::moduleBase + 0x1b1032;

	auto iac2addr = (char*)(Core::moduleBase + 0x1b1028);
	MH_CreateHook((LPVOID)iac2addr, &ItemAvailableClick2Hook, reinterpret_cast<LPVOID*>(&ItemAvailableClick2Origin));
	
	ChunkLoadedOriginReturnAddr = Core::moduleBase + 0x17dec2+5;
	auto claddr = (char*)(Core::moduleBase + 0x17dec2);
	MH_CreateHook((LPVOID)claddr, &ChunkLoadedHook, reinterpret_cast<LPVOID*>(&ChunkLoadedOrigin));

	TriggerHitOriginReturnAddr = Core::moduleBase + 0x1d0b35;
	auto thaddr = (char*)(Core::moduleBase + 0x1d0b2e);
	MH_CreateHook((LPVOID)thaddr, &TriggerHitHook, reinterpret_cast<LPVOID*>(&TriggerHitOrigin));

	auto titleaddr = (char*)(Core::moduleBase + 0x32E5F0);
	MH_CreateHook((LPVOID)titleaddr, &HookedGetString, reinterpret_cast<void**>(&originalGetString));
}

bool GameHandler::OnItemAvailable(void* itemPtr) {
	uint8_t* base = static_cast<uint8_t*>(itemPtr);
	short id = *reinterpret_cast<short*>(base + 0x4);
	return APSaveData::hasBoughtItem(id);
}

static std::string myCustomTitle = "AP Item Name";
static std::string PadDisabled = "Pad Disabled By AP";
static std::string copyright = "Krome Studios Inc.  All rights reserved.  TY the Tasmanian Tiger, Bush Rescue and characters and the Krome Studios logo are trademarks of Krome Studios Inc.\n\nAP Mod Created By\nFyreDay\n xMcacutt Dashieswag92";

int __cdecl GameHandler::HookedGetString(int param_1) {
	API::LogPluginMessage(std::to_string(param_1));
	if (param_1 == 564) {
		return (int)myCustomTitle.c_str(); // Make sure string stays alive
	}
	if (param_1 == 1) {
		return (int)copyright.c_str();
	}

	if(param_1 == 38){
		uintptr_t objptr = MKObject::GetMKObject(100);
		bool* canLeave = reinterpret_cast<bool*>(objptr + 0x3f0);
		if (!*canLeave) {
			return (int)PadDisabled.c_str();
		}
	}

	return originalGetString(param_1);
}

void GameHandler::OnChunkLoaded() {
	for (int pad : APSaveData::AllParkingPads) {
		
		if (std::find(APSaveData::UnlockedParkingPads.begin(), APSaveData::UnlockedParkingPads.end(), pad) != APSaveData::UnlockedParkingPads.end()){
			uintptr_t objptr = MKObject::GetMKObject(pad);
			if (objptr != 0) {
				*(short*)(objptr + 0x1a) = 0x0016;
				API::LogPluginMessage(std::to_string(pad) + " enabled");
			}
		}
		else {
			uintptr_t objptr = MKObject::GetMKObject(pad);
			if (objptr != 0) {
				*(short*)(objptr + 0x1a) = 0x0000;
				API::LogPluginMessage(std::to_string(pad) + " disabled");
			}
		}
	}
	
	API::LogPluginMessage("OnChunkLoaded");
}



void GameHandler::TryDisableFourbieTrigger() {
	int* closestID = reinterpret_cast<int*>(Core::moduleBase + 0x4BDD00);
	bool canExit = false;
	if (std::find(APSaveData::UnlockedParkingPads.begin(), APSaveData::UnlockedParkingPads.end(), *closestID) != APSaveData::UnlockedParkingPads.end()) {
		canExit = true;
	}
	uintptr_t objptr = MKObject::GetMKObject(100);
	if (objptr != 0) {
		API::LogPluginMessage("Found Car");
		*(bool*)(objptr + 0x3f0) = canExit;
	}
}

void GameHandler::TryEditFourbieTrigger(bool enable) {


	// Follow the pointer chain: ty2.exe+4ED1C8 -> [ +0x8 ] -> [ +0x8 ] -> string
	uintptr_t* p1 = *reinterpret_cast<uintptr_t**>(Core::moduleBase + 0x4ED1C8);
	if (!p1) {
		API::LogPluginMessage("No Trigger");
		return;
	}

	uintptr_t* p2 = *reinterpret_cast<uintptr_t**>(reinterpret_cast<uintptr_t>(p1) + 0x8);
	if (!p2) return;

	const char* triggerName = *reinterpret_cast<const char**>(reinterpret_cast<uintptr_t>(p2) + 0x8);
	if (!triggerName) return;

	if (strcmp(triggerName, "FourbieTriggerProp") == 0) {
		// Now set *(bool*)(p1 + 0x24) = false;
		bool* flagPtr = reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(p1) + 0x24);
		*flagPtr = enable;
		API::LogPluginMessage("Successfully eddited trigger flag at 0x24.");
		if (!enable) {

		}
	}
	else {
		API::LogPluginMessage("Trigger name does not match.");
	}
}

/*
0057da30 load chunk
*/