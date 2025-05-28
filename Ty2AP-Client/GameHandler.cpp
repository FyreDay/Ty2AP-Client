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

char* saveFileBuffer = nullptr;
int saveFileLength = 0;

FunctionType LoadSaveFileOrigin = nullptr;
uintptr_t LoadSaveFileOriginReturnAddr;
__declspec(naked) void __stdcall GameHandler::LoadSaveFileHook() {
	_asm {
		call GameHandler::LoadAPSaveFile
		add esp, 0xC

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

	//////load
	/*LoadSaveFileOriginReturnAddr = Core::moduleBase + 0x376e84+5;
	auto loadaddr = (char*)(Core::moduleBase + 0x376e84);
	MH_CreateHook((LPVOID)loadaddr, &LoadSaveFileHook, reinterpret_cast<LPVOID*>(&LoadSaveFileOrigin));*/

	SaveFileOriginReturnAddr = Core::moduleBase + 0x376d6f+5;
	auto saveaddr = (char*)(Core::moduleBase + 0x376d6f);
	MH_CreateHook((LPVOID)saveaddr, &SaveFileHook, reinterpret_cast<LPVOID*>(&SaveFileOrigin));

	//PatchStartingLevel();
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

bool GameHandler::OnItemAvailable(void* itemPtr) {
	uint8_t* base = static_cast<uint8_t*>(itemPtr);
	short id = *reinterpret_cast<short*>(base + 0x4);
	return APSaveData::hasBoughtItem(id);
}

static std::string myCustomTitle = "AP Item Name";
static std::string PadDisabled = "Pad Disabled By AP";
static std::string copyright = "Krome Studios Inc.  All rights reserved.  TY the Tasmanian Tiger, Bush Rescue and characters and the Krome Studios logo are trademarks of Krome Studios Inc.\n\nAP Mod Created By\nFyreDay\n xMcacutt Dashieswag92";

int __cdecl GameHandler::HookedGetString(int param_1) {
	//API::LogPluginMessage(std::to_string(param_1));
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

bool fileExists(const std::string& filePath) {
	std::ifstream file(filePath);
	return file.good();
}

void createDirectoriesIfNeeded(const std::string& filepath) {
	std::filesystem::path path(filepath);
	std::filesystem::create_directories(path.parent_path());
}


void GameHandler::LoadAPSaveFile() {
	auto filePath = "./Saves/" + std::to_string(5) + "_" + "fyre";
	createDirectoriesIfNeeded(filePath);
	std::ifstream file(filePath, std::ios::binary);
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
		//set state to loading
		*(DWORD*)(Core::moduleBase + 0x4CBD78 + 0x238) = 1;
		//this doesnt work
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
			API::LogPluginMessage("result: " + std::to_string(result));
		};

	}
	else {
		API::LogPluginMessage("Save file not found.");
		saveFileLength = 0;
		saveFileBuffer = nullptr;
	}
}

//using CallbackFn = int(__stdcall*)(void* dest, void* src, int size);
//CallbackFn fn = (CallbackFn)(Core::moduleBase + 0x3b7e10);

//void** destPtrLocation = reinterpret_cast<void**>(Core::moduleBase + 0x4CBD78 + 0x74);
//void* dest = *destPtrLocation;
//// Call the function
//int result = fn(dest, saveFileBuffer, saveFileLength);
//API::LogPluginMessage("result: " + std::to_string(result));
//g_SaveCallback.active = true;
//g_SaveCallback.esi = Core::moduleBase + 0x4CBD78;
//g_SaveCallback.framesRemaining = 5; // delay ~5 frames

int GameHandler::SaveFile(const char* filename, void* data, int size) {
	std::string filePath = "./Saves/" + std::to_string(5) + "_" + "fyre";
	createDirectoriesIfNeeded(filePath);
	FILE* f = nullptr;
	if (fopen_s(&f, filePath.c_str(), "wb") == 0 && f) {
		fwrite(data, 1, size, f);
		fclose(f);
		API::LogPluginMessage("Save file Saved.");
	}


	g_SaveCallback.esi = Core::moduleBase + 0x4CBD78;
	g_SaveCallback.framesRemaining = 5; // delay ~5 frames
	g_SaveCallback.active = true;
	// Define callback type, assuming __thiscall (ECX = this)
	//typedef void(__thiscall* SteamCallbackType)(void* thisptr, void* param1, int param2);
	//SteamCallbackType steamCallback = (SteamCallbackType)FUN_00777a10;

	// Otherwise, return success manually
	return 1;  // pretend success
}
/*
0057da30 load chunk
269200 handles game state
0854d74 is z2 hq chunck
*/