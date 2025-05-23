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
//FunctionType ChunkLoadedOrigin = nullptr;
//uintptr_t ChunkLoadedOriginReturnAddr;
//__declspec(naked) void __stdcall GameHandler::ChunkLoadedHook() {
//	_asm {
//		push ebx
//		push edi
//		push esi
//		push edx
//		push ecx
//		push eax
//		call CheckHandler::OnBuyItem
//		pop eax
//		pop ecx
//		pop edx
//		pop esi
//		pop edi
//		pop ebx
//		push 0x2EE
//		jmp dword ptr[PurchaseItemOriginReturnAddr]
//	}
//}


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
	
	auto titleaddr = (char*)(Core::moduleBase + 0x32E5F0);
	MH_CreateHook((LPVOID)titleaddr, &HookedGetString, reinterpret_cast<void**>(&originalGetString));

	//hook 00699990 at 699a1e to change how items are required
}

bool GameHandler::OnItemAvailable(void* itemPtr) {
	uint8_t* base = static_cast<uint8_t*>(itemPtr);
	short id = *reinterpret_cast<short*>(base + 0x4);
	return APSaveData::hasBoughtItem(id);
	//API::LogPluginMessage("has Infra");
}

static std::string myCustomTitle = "AP Item Name";

int __cdecl GameHandler::HookedGetString(int param_1) {
	if (param_1 == 564) {
		return (int)myCustomTitle.c_str(); // Make sure string stays alive
	}

	return originalGetString(param_1);
}
/*
0057da30 load chunk
*/