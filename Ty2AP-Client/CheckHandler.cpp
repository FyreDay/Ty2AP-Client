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
		cmp edx, 05
		jne not_equal
		
		jmp dword ptr[CompleteMissionOriginReturnAddr]
		not_equal:
		jmp dword ptr[CompleteMissionBranchReturnAddr]
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
	


	for (auto& window : GUI::windows) {
		if (auto infowindow = dynamic_cast<InfoWindow*>(window.get())) {
			infowindow->AddLogMessage(collectibles[type].name + " " +  std::to_string(id));
		}
	}
	//API::LogPluginMessage("maybe id: " + std::to_string(id) + " maybe type: " + std::to_string(type));
}

void CheckHandler::OnCompleteMission(void* mission, int status) {
	if (status != 5) {
		return;
	}
	uint8_t* base = static_cast<uint8_t*>(mission);
	//short status = *reinterpret_cast<short*>(base + 0x10); // read status
	
	int id = *reinterpret_cast<int*>(base + 0x4);
	ArchipelagoHandler::SendLocation(id);
	
	short value = *reinterpret_cast<short*>(base + 0x4); // read short

	if (value == 83) { //this is the cass boss fight id
		API::LogPluginMessage("GOALLLLL");
		ArchipelagoHandler::gameFinished();
	}
	char letter = *reinterpret_cast<char*>(base + 0x7);  // read char
	std::string Sid = std::string(1, letter) + std::to_string(value);
	for (auto& window : GUI::windows) {
		if (auto infowindow = dynamic_cast<InfoWindow*>(window.get())) {
			infowindow->AddLogMessage(Sid + " -> " + std::to_string(status));
		}
	}
	int missioncount = 0;
	SaveData::MissionList(5).forEach([&missioncount](MissionStruct m) {
		if (m.id < 100 && m.id != 86) {
			missioncount++;
		}
	});
	API::LogPluginMessage("misions done: " + std::to_string(missioncount) + " Need: " + std::to_string(ArchipelagoHandler::slotdata->missionsToGoal));
	if (missioncount >= ArchipelagoHandler::slotdata->missionsToGoal - 1) {
		std::optional<MissionStruct> mission = SaveData::findMissionByID(99);
		if (mission.has_value() && mission.value().status == 0) {
			mission.value().numberPreconditionMissionNeeded = 0;
			Missions::UpdateMissionState(&mission.value(), 1, 0);
		}
	}

	
}

void CheckHandler::OnBuyItem(void* item) {
	uint8_t* base = static_cast<uint8_t*>(item);
	//short status = *reinterpret_cast<short*>(base + 0x10); // read status

	short value = *reinterpret_cast<short*>(base + 0x4); // read short
	char letter = *reinterpret_cast<char*>(base + 0x7);  // read char
	ArchipelagoHandler::customSaveData->updateBoughtItem(value, false);
	
	ArchipelagoHandler::SendLocation(value);

	std::string id = std::string(1, letter) + std::to_string(value);
	for (auto& window : GUI::windows) {
		if (auto infowindow = dynamic_cast<InfoWindow*>(window.get())) {
			infowindow->AddLogMessage("Buy " + id);
		}
	}
}


