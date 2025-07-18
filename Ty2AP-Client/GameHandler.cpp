#include "pch.h"
#include "GameHandler.h"
using nlohmann::json;
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

char* saveFileBuffer = nullptr;
int saveFileLength = 0;

FunctionType LoadSaveFileOrigin = nullptr;
uintptr_t LoadSaveFileOriginReturnAddr;
__declspec(naked) void __stdcall GameHandler::LoadSaveFileHook() {
	_asm {
		add esp, 0x8
		call GameHandler::LoadAPSaveFile

		mov eax, saveFileLength
		mov[edi + 0x68], eax

		sub eax, 0xc
		mov[edi + 0x70], eax

		mov esi, saveFileBuffer       // Source (loaded file)
		mov edi, [edi + 0x6C]         // Destination (preallocated buffer)
		mov ecx, saveFileLength
		rep movsb
		jmp dword ptr[LoadSaveFileOriginReturnAddr]
	}
}

FunctionType GetSaveDataSize1Origin = nullptr;
uintptr_t GetSaveDataSize1OriginReturnAddr;
__declspec(naked) void __stdcall GameHandler::GetSaveDataSize1Hook() {
	_asm {
		call GameHandler::GetSaveDataSize

		jmp dword ptr[GetSaveDataSize1OriginReturnAddr]
	}
}

FunctionType GetSaveDataSize2Origin = nullptr;
uintptr_t GetSaveDataSize2OriginReturnAddr;
__declspec(naked) void __stdcall GameHandler::GetSaveDataSize2Hook() {
	_asm {
		call GameHandler::GetSaveDataSize

		jmp dword ptr[GetSaveDataSize2OriginReturnAddr]
	}
}

FunctionType SaveFileOrigin = nullptr;
uintptr_t SaveFileOriginReturnAddr;
__declspec(naked) void __stdcall GameHandler::SaveFileHook() {
	_asm {
		call GameHandler::SaveFile
		add esp, 0xC
		jmp dword ptr[SaveFileOriginReturnAddr]
	}
}

typedef int(__cdecl* TGetStringFunc)(int param_1);
TGetStringFunc originalGetString = nullptr;

typedef void(__thiscall* StateTransitionFunc)(void* thisPtr, int firstArg, int secondArg);
StateTransitionFunc origionalStateTransition = nullptr;

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

	auto deathaddr = (char*)(Core::moduleBase + 0x22c7d0);
	MH_CreateHook((LPVOID)deathaddr, &DeathHook, reinterpret_cast<void**>(&origionalStateTransition));

	//load custom save
	LoadSaveFileOriginReturnAddr = Core::moduleBase + 0x376e84+5;
	auto loadaddr = (char*)(Core::moduleBase + 0x376e84);
	MH_CreateHook((LPVOID)loadaddr, &LoadSaveFileHook, reinterpret_cast<LPVOID*>(&LoadSaveFileOrigin));
	
	//modify size of slot buffer in both places
	GetSaveDataSize1OriginReturnAddr = Core::moduleBase + 0x2697af +7;
	auto size1addr = (char*)(Core::moduleBase + 0x2697af);
	MH_CreateHook((LPVOID)size1addr, &GetSaveDataSize1Hook, reinterpret_cast<LPVOID*>(&GetSaveDataSize1Origin));

	GetSaveDataSize2OriginReturnAddr = Core::moduleBase + 0x234492 +7;
	auto size2addr = (char*)(Core::moduleBase + 0x234492);
	MH_CreateHook((LPVOID)size2addr, &GetSaveDataSize2Hook, reinterpret_cast<LPVOID*>(&GetSaveDataSize2Origin));

	NopInstructions((void*)(Core::moduleBase + 0x376e67), 29);
	//remove the steam callbackandload setup
	NopInstructions((void*)(Core::moduleBase + 0x376eaf), 12);
	NopInstructions((void*)(Core::moduleBase + 0x376ece), 6);

	//remove the steam finish saveload code
	NopInstructions((void*)(Core::moduleBase + 0x377b34), 20);
	NopInstructions((void*)(Core::moduleBase + 0x377b4b), 8);
	//save
	SaveFileOriginReturnAddr = Core::moduleBase + 0x376d6f+5;
	auto saveaddr = (char*)(Core::moduleBase + 0x376d6f);
	MH_CreateHook((LPVOID)saveaddr, &SaveFileHook, reinterpret_cast<LPVOID*>(&SaveFileOrigin));
	


	//remove write to collectablecount
	NopInstructions((void*)(Core::moduleBase + 0x11b2e1), 6);
	}

void GameHandler::PatchStartingLevel() {
	// Address of the instruction to patch (PUSH 0x00851738)
	uintptr_t address = Core::moduleBase + 0x234504;

	// New bytes: PUSH 0x00454D74 -> 68 74 4D 45 00
	uintptr_t stringAddr = Core::moduleBase + 0x454D74;

	BYTE patch[5];
	patch[0] = 0x68; // PUSH opcode
	*reinterpret_cast<uint32_t*>(&patch[1]) = static_cast<uint32_t>(stringAddr);
	// Unprotect memory region
	DWORD oldProtect;
	if (VirtualProtect(reinterpret_cast<LPVOID>(address), sizeof(patch), PAGE_EXECUTE_READWRITE, &oldProtect)) {
		// Apply patch
		memcpy(reinterpret_cast<void*>(address), patch, sizeof(patch));

		// Restore old protection
		VirtualProtect(reinterpret_cast<LPVOID>(address), sizeof(patch), oldProtect, &oldProtect);
		std::cout << "Patch applied successfully." << std::endl;
	}
	else {
		std::cerr << "Failed to change memory protection." << std::endl;
	}
}

void GameHandler::NopInstructions(void* address, size_t size) {
	DWORD oldProtect;
	VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memset(address, 0x90, size); // 0x90 = NOP
	VirtualProtect(address, size, oldProtect, &oldProtect);
	FlushInstructionCache(GetCurrentProcess(), address, size);
}

bool GameHandler::OnItemAvailable(void* itemPtr) {
	uint8_t* base = static_cast<uint8_t*>(itemPtr);
	short id = *reinterpret_cast<short*>(base + 0x4);
	return ArchipelagoHandler::customSaveData->hasBoughtItem(id);
}

static std::string PadDisabled = "Pad Disabled By AP";
static std::string copyright = "Krome Studios Inc.  All rights reserved.  TY the Tasmanian Tiger, Bush Rescue and characters and the Krome Studios logo are trademarks of Krome Studios Inc.\n\nAP Mod Created By\nFyreDay\n\nSpecial Thanks\nxMcacutt Dashieswag92";
static std::string selectsave = "Select any save game. Instead of Loading Vanilla, custom AP Save data will be loaded/Created instead";
static std::string overwrite = "Creating new custom save data. Ok?";

int __cdecl GameHandler::HookedGetString(int param_1) {
	//API::LogPluginMessage(std::to_string(param_1));
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
	int* state = reinterpret_cast<int*>(Core::moduleBase + 0x4EB508);
	if (*state == 2) {
		const char* itemname = ItemHandler::GetShopItemName(param_1);
		if (itemname) {
			return (int)itemname;
		}
	}
	if (param_1 == 78) {
		return (int)selectsave.c_str();
	}
	if (param_1 == 93) {
		return (int)overwrite.c_str();
	}
	//API::LogPluginMessage(std::to_string(param_1));
	return originalGetString(param_1);
}

void GameHandler::OnChunkLoaded() {
	
	for (int pad : ArchipelagoHandler::customSaveData->AllParkingPads) {
		
		if (std::find(ArchipelagoHandler::customSaveData->UnlockedParkingPads.begin(), ArchipelagoHandler::customSaveData->UnlockedParkingPads.end(), pad) != ArchipelagoHandler::customSaveData->UnlockedParkingPads.end()){
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
	if (std::find(ArchipelagoHandler::customSaveData->UnlockedParkingPads.begin(), ArchipelagoHandler::customSaveData->UnlockedParkingPads.end(), *closestID) != ArchipelagoHandler::customSaveData->UnlockedParkingPads.end()) {
		canExit = true;
	}
	uintptr_t objptr = MKObject::GetMKObject(100);
	if (objptr != 0) {
		*(bool*)(objptr + 0x3f0) = canExit;
	}
}

void GameHandler::TryEditFourbieTrigger(bool enable) {


	// Follow the pointer chain: ty2.exe+4ED1C8 -> [ +0x8 ] -> [ +0x8 ] -> string
	uintptr_t* p1 = *reinterpret_cast<uintptr_t**>(Core::moduleBase + 0x4ED1C8);
	if (!p1) {
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
	}
	else {
		API::LogPluginMessage("Trigger name does not match.");

	}
}

bool fileExists(const std::string& filePath) {
	std::ifstream file(filePath);
	return file.good();
}

void createDirectoriesIfNeeded(const std::string& filepath) {
	std::filesystem::path path(filepath);
	std::filesystem::create_directories(path.parent_path());
}


int GameHandler::GetSaveDataSize() {
	int size = 0;
	std::string filePath = "./Saves/" + ArchipelagoHandler::GetSaveIdentifier();
	createDirectoriesIfNeeded(filePath);
	std::ifstream file(filePath + ".bin", std::ios::binary);
	if (file.is_open()) {
		
		file.read(reinterpret_cast<char*>(&size), sizeof(int));
		file.close();
	}
	return size;
}

bool GameHandler::doesSaveExist() {
	std::string filePath = "./Saves/" + ArchipelagoHandler::GetSaveIdentifier() + ".json";
	std::ifstream file(filePath);
	return file.good();
}

void GameHandler::LoadAPSaveFile() {
	std::string filePath = "./Saves/" + ArchipelagoHandler::GetSaveIdentifier();
	API::LogPluginMessage(filePath);
	createDirectoriesIfNeeded(filePath);
	std::ifstream file(filePath + ".bin", std::ios::binary);
	if (file.is_open()) {
		file.seekg(0, std::ios::end);
		saveFileLength = file.tellg();
		file.seekg(0, std::ios::beg);
		if (saveFileLength > 0) {
			// Allocate buffer and read the rest of the file
			saveFileBuffer = new char[saveFileLength];
			if (!file.read(saveFileBuffer, saveFileLength)) {
				delete[] saveFileBuffer;
				saveFileBuffer = nullptr;
				saveFileLength = 0;
				API::LogPluginMessage("Failed to read save file.");
			}
		}

		file.close();

		read_json_file(filePath + ".json");

		*(DWORD*)(Core::moduleBase + 0x4CBD78 + 0x238) = 1;
		
		g_SaveCallback.active = true;
		g_SaveCallback.esi = Core::moduleBase + 0x4CBD78;
		g_SaveCallback.framesRemaining = 5;
		g_SaveCallback.callback = []() {
			using CallbackFn = int(__thiscall*)(void* thisptr, void* arg1, int arg2);
			CallbackFn fn = (CallbackFn)(Core::moduleBase + 0x377AE0);

			void* thisptr = (void*)(Core::moduleBase + 0x4CBD78);

			uint8_t dataBuffer[] = {
				0x46, 0xC1, 0xF7, 0xCD,
				0x18, 0x74, 0xD7, 0x09,
				0x01, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00
			};

			dataBuffer[0x10] = (uint8_t)(saveFileLength & 0xFF);
			dataBuffer[0x11] = (uint8_t)((saveFileLength >> 8) & 0xFF);
			dataBuffer[0x12] = (uint8_t)((saveFileLength >> 16) & 0xFF);
			dataBuffer[0x13] = (uint8_t)((saveFileLength >> 24) & 0xFF);

			int arg2 = 0;

			// Call the function
			int result = fn(thisptr, dataBuffer, arg2);
		};

	}
	else {
		API::LogPluginMessage("Save file not found.");
		saveFileLength = 0;
		saveFileBuffer = nullptr;
	}


}

int GameHandler::SaveFile(const char* filename, void* data, int size) {
	std::string filePath = "./Saves/" +  ArchipelagoHandler::GetSaveIdentifier();
	createDirectoriesIfNeeded(filePath);
	FILE* f = nullptr;
	if (fopen_s(&f, (filePath + ".bin").c_str(), "wb") == 0 && f) {
		fwrite(data, 1, size, f);
		fclose(f);
		API::LogPluginMessage("Save file Saved.");
	}

	write_json_file(filePath + ".json");

	g_SaveCallback.esi = Core::moduleBase + 0x4CBD78;
	g_SaveCallback.framesRemaining = 5; // delay 5 frames
	g_SaveCallback.active = true;

	return 1; 
}

void GameHandler::write_json_file(const std::string& filename) {
	json j;

	// Populate the JSON object
	j["LastRecievedIndex"] = ArchipelagoHandler::customSaveData->pLastReceivedIndex;
	j["ShopData"] = ArchipelagoHandler::customSaveData->ItemMap;
	j["UnlockedParkingBays"] = ArchipelagoHandler::customSaveData->UnlockedParkingPads;

	// Write to file
	std::ofstream file(filename);
	if (file.is_open()) {
		file << j.dump(4);  // pretty print with indent of 4
		file.close();
	}
	else {
		std::cerr << "Could not open file for writing: " << filename << std::endl;
	}
}

void GameHandler::read_json_file(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		return;
	}

	json j;
	file >> j;

	// Access data
	ArchipelagoHandler::customSaveData->pLastReceivedIndex = j["LastRecievedIndex"];
	ArchipelagoHandler::customSaveData->ItemMap = j["ShopData"].get<std::map<int, bool>>();
	ArchipelagoHandler::customSaveData->UnlockedParkingPads = j["UnlockedParkingBays"].get<std::list<int>>();
}

bool GameHandler::IsInGame() {
	//uintptr_t addr = Core::moduleBase + 0x4EB510;
	uintptr_t addr = Core::moduleBase + 0x4F3888;
	char buffer[9] = { 0 }; // Ensure null-terminated
	memcpy(buffer, reinterpret_cast<const void*>(addr), 8);

	return strncmp(buffer, "mainmenu", 8) != 0;
}

bool GameHandler::hasRunSetup = false;
void GameHandler::RunLoadSetup(SlotData* slotdata) {
	ItemHandler::HandleStoredItems();

	int index = 0;
	SaveData::ItemList(1).forEach([&index, slotdata](ItemWrapper item) {
		if (item.getCurrencyType() == 0) {
			item.setPrice(slotdata->copPrices[index]);
			item.setPuchusedStatus(!ArchipelagoHandler::customSaveData->hasBoughtItem(item.getID()));
			index++;
		}
	});

	index = 0;
	SaveData::ItemList(1).forEach([&index, slotdata](ItemWrapper item) {
		if (item.getCurrencyType() == 2) {
			item.setPrice(slotdata->cogPrices[index]);
			item.setPuchusedStatus(!ArchipelagoHandler::customSaveData->hasBoughtItem(item.getID()));
			if (item.getID() > 79) {
				item.setLocked(ArchipelagoHandler::customSaveData->hasBoughtItem(item.getID() - 1));
			}

			index++;
		}
	});

	index = 0;
	SaveData::ItemList(2).forEach([&index, slotdata](ItemWrapper item) {
		if (item.getCurrencyType() == 0) {
			item.setPrice(slotdata->rangPrices[index]);
			item.setPuchusedStatus(!ArchipelagoHandler::customSaveData->hasBoughtItem(item.getID()));
			index++;
		}
	});

	index = 0;
	SaveData::ItemList(3).forEach([&index, slotdata](ItemWrapper item) {
		if (item.getCurrencyType() == 0) {
			item.setPrice(slotdata->slyPrices[index]);
			item.setPuchusedStatus(!ArchipelagoHandler::customSaveData->hasBoughtItem(item.getID()));
			index++;
		}
	});

	index = 0;
	SaveData::ItemList(4).forEach([&index, slotdata](ItemWrapper item) {
		if (item.getCurrencyType() == 1) {
			item.setPrice(slotdata->orbPrices[index]);
			item.setPuchusedStatus(!ArchipelagoHandler::customSaveData->hasBoughtItem(item.getID()));
			if (item.getID() > 5) {
				item.setLocked(ArchipelagoHandler::customSaveData->hasBoughtItem(item.getID() - 1));
			}
			
			index++;
		}
	});

	SetMissionRequirements(ArchipelagoHandler::slotdata->barrierUnlockStyle, ArchipelagoHandler::slotdata->missionsToGoal);

	hasRunSetup = true;
}

void GameHandler::SetMissionRequirements(BarrierUnlock unlockType, int mission_goal) {
	SaveData::MissionList(0).forEach([](MissionWrapper mission) { //52 is oil rig
		if (mission.getID() == 99) {
			mission.setNumberOfMissionsRequired(44);
		}
		else  if (mission.getID() != 83 and mission.getID() != 82) { //82 and 83 have only 1 precondition that I dont want to mess with
			mission.setNumberOfMissionsRequired(0);
		}
	});
}

void __fastcall GameHandler::DeathHook(void* thisptr, int edx, int state, int source) {
	API::LogPluginMessage("State: " + std::to_string(state) + " Source: " + std::to_string(source));
	if (source != 9000 && state == 0xe) {
		ArchipelagoHandler::SendDeath();
	}

	origionalStateTransition(thisptr, state, source);
}

void GameHandler::KillTy() {
	auto transitionFunc = (StateTransitionFunc)(Core::moduleBase + 0x0022c7d0);
	void* tyStateHandler = (void*)(MKObject::GetMKObject(204) + 0x470);
	int stateid = 0xe;
	int source = 9000;
	transitionFunc(tyStateHandler, stateid, source);
}

void GameHandler::EnableLoadButtons()
{
	if (IsInGame()) {
		return;
	}
	auto buttongroup = MKUI::FindChildElementByName(MKUI::GetMainMenu(), "MainMenuButtons");
	if (buttongroup == nullptr) {
		API::LogPluginMessage("Button group does not exist here");
		return;
	}
	if (doesSaveExist()) {
		UIElementStruct* loadgame = MKUI::FindChildElementByName(buttongroup, "LoadGame");
		if (loadgame != nullptr) {
			loadgame->interactable = true;
			loadgame->enabled = true;
			loadgame->Red = 0.63f;
			loadgame->Blue = 0.63f;
			loadgame->Green = 0.63f;
		}
	}
	else {
		UIElementStruct* newgame = MKUI::FindChildElementByName(buttongroup, "NewGame");
		if (newgame != nullptr) {
			newgame->interactable = true;
			newgame->enabled = true;
			newgame->Red = 0.63f;
			newgame->Blue = 0.63f;
			newgame->Green = 0.63f;
		}
	}
}


void GameHandler::DisableLoadButtons()
{
	auto buttongroup = MKUI::FindChildElementByName(MKUI::GetMainMenu(), "MainMenuButtons");
	if (buttongroup == nullptr) {
		return;
	}
	UIElementStruct* loadgame = MKUI::FindChildElementByName(buttongroup, "LoadGame");
	if (loadgame != nullptr) {
		loadgame->interactable = false;
		loadgame->enabled = false;
		loadgame->currentlySelected = false;
		loadgame->Red = 0.4f;
		loadgame->Blue = 0.4f;
		loadgame->Green = 0.4f;
		API::LogPluginMessage("Disable load game");
	}
	UIElementStruct* newgame = MKUI::FindChildElementByName(buttongroup, "NewGame");
	if (newgame != nullptr) {
		newgame->interactable = false;
		newgame->enabled = false;
		newgame->currentlySelected = false;
		newgame->Red = 0.4f;
		newgame->Blue = 0.4f;
		newgame->Green = 0.4f;
	}
}

/*
0057da30 load chunk
269200 handles game state
0854d74 is z2 hq chunck
*/