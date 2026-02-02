#include "pch.h"
#include "CheckHandler.h"
typedef void(__stdcall* FunctionType)();
FunctionType CollectCollectibleOrigin = nullptr;
uintptr_t CollectCollectibleOriginReturnAddr;

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

FunctionType CompleteMissionOrigin = nullptr;
uintptr_t CompleteMissionOriginReturnAddr;
uintptr_t CompleteMissionBranchReturnAddr;
uintptr_t CompleteMissionConditionReturnAddr;

static bool __stdcall IsVanilla() {
	return ArchipelagoHandler::slotdata->barrierUnlockStyle == BarrierUnlock::Vanilla;
}

__declspec(naked) void __stdcall CheckHandler::CompleteMissionHook() {
	_asm {
		pushfd
		pushad
		push edx
		push ecx
		call CheckHandler::OnCompleteMission
		pop ecx
		pop edx
		popad
		popfd
		call IsVanilla
		cmp al, 1
		je is_vanilla
		push eax
		movzx eax, word ptr[ecx+0x4]
		cmp eax, 980
		je barrier
		cmp eax, 981
		je barrier
		cmp eax, 982
		je barrier
		pop eax
	is_vanilla:
		cmp edx, 5
		jne not_equal
		jmp dword ptr[CompleteMissionOriginReturnAddr]
	not_equal:
		jmp dword ptr[CompleteMissionBranchReturnAddr]
	barrier:
		pop eax
		push esi 
		mov esi,[ecx+0x2C]
		jmp dword ptr[CompleteMissionConditionReturnAddr]
	}
}

FunctionType PurchaseItemOrigin = nullptr;
uintptr_t PurchaseItemOriginReturnAddr;

__declspec(naked) void __stdcall CheckHandler::PurchaseItemHook() {
	_asm {
		push ebx
		push edi
		push esi
		push edx
		push ecx
		push eax
		call CheckHandler::OnBuyItem
		pop eax
		pop ecx
		pop edx
		pop esi
		pop edi
		pop ebx
		push 0x2EE
		jmp dword ptr[PurchaseItemOriginReturnAddr]
	}
}

void CheckHandler::SetupHooks() {
	CollectCollectibleOriginReturnAddr = Core::moduleBase + 0x0011b2ba + 5;
	auto ccaddr = (char*)(Core::moduleBase + 0x0011b2ba);
	MH_CreateHook((LPVOID)ccaddr, &CollectCollectibleHook, reinterpret_cast<LPVOID*>(&CollectCollectibleOrigin));

	CompleteMissionOriginReturnAddr = Core::moduleBase + 0x0029734f + 5;
	CompleteMissionBranchReturnAddr = Core::moduleBase + 0x00297364;
	CompleteMissionConditionReturnAddr = Core::moduleBase + 0x29736b;
	auto cmaddr = (char*)(Core::moduleBase + 0x0029734f);
	MH_CreateHook((LPVOID)cmaddr, &CompleteMissionHook, reinterpret_cast<LPVOID*>(&CompleteMissionOrigin));

	PurchaseItemOriginReturnAddr = Core::moduleBase + 0x001b1468;
	auto piaddr = (char*)(Core::moduleBase + 0x001b145c);
	MH_CreateHook((LPVOID)piaddr, &PurchaseItemHook, reinterpret_cast<LPVOID*>(&PurchaseItemOrigin));
}

void CheckHandler::OnCollectCollectible(int type, int id) {
	//dont send on sanity
	switch (type) {
	case 0: ArchipelagoHandler::SendLocation(('C' << 8) | id); break;
	case 1: ArchipelagoHandler::SendLocation(('B' << 8) | id); break;
	case 2: ArchipelagoHandler::SendLocation(('P' << 8) | id); break;
	case 3: ArchipelagoHandler::SendLocation(('K' << 8) | id); break;
	case 4: ArchipelagoHandler::SendLocation(('S' << 8) | id); break;
	case 5: ArchipelagoHandler::SendLocation(('F' << 8) | id); break;
	default:
		API::LogPluginMessage("Failure on collectable Type");
	}
}

void CheckHandler::OnCompleteMission(void* mission, int status) {
	if (status != 5) {
		return;
	}

	uint8_t* base = static_cast<uint8_t*>(mission);

	int id = *reinterpret_cast<int*>(base + 0x4);
	int shortId = *reinterpret_cast<char*>(base + 0x4);

	if (shortId >= 980 && shortId <= 982) {
		return;
	}

	short value = *reinterpret_cast<short*>(base + 0x4); // read short
	if (value == Mission::FINAL_BOSS) {
		API::LogPluginMessage("GOALLLLL");
		ArchipelagoHandler::gameFinished();
	}

	ArchipelagoHandler::SendLocation(id);
	char letter = *reinterpret_cast<char*>(base + 0x7);  // read char

	ArchipelagoHandler::customSaveData->CheckGoal();
}

void CheckHandler::OnBuyItem(void* item) {
	uint8_t* base = static_cast<uint8_t*>(item);

	short value = *reinterpret_cast<short*>(base + 0x4); // read short
	char letter = *reinterpret_cast<char*>(base + 0x7);  // read char
	ArchipelagoHandler::customSaveData->updateBoughtItem(value, true);
	if (value >= 79 && value < 88) {
		if (auto nextItem = SaveData::findItemByID(SaveData::GetShopItemList(1), value + 1)) {
			nextItem->locked = false;
		}
	}

	if (value >= 5 && value < 7) {
		if (auto nextItem = SaveData::findItemByID(SaveData::GetShopItemList(4), value + 1)) {
			nextItem->locked = false;
		}
	}

	ArchipelagoHandler::SendLocation(value);
}