#pragma once
#include "ArchipelagoHandler.h"
#include "CheckHandler.h"
#include "APSaveData.h"
#include <windows.h>
#include "Mission.h"

typedef int(__cdecl* GetStringFunc)(int id);
extern GetStringFunc originalGetString;

class GameHandler
{
public:
	struct SaveCallbackInfo {
		std::function<void()> callback;
		DWORD esi;
		int framesRemaining;
		bool active;
	};
	static inline SaveCallbackInfo g_SaveCallback = { nullptr,0, 0, false };
	static void Initialize();
	static void SetupHooks();

	static void __stdcall ItemAvailableUIHook();
	static void __stdcall ItemAvailableLogicHook();
	static void __stdcall ItemAvailableClickHook();
	static void __stdcall ItemAvailableClick2Hook();

	static void __stdcall ChunkLoadedHook();
	static void __stdcall TriggerHitHook();

	static void __stdcall LoadSaveFileHook();
	static void __stdcall GetSaveDataSize1Hook();
	static void __stdcall GetSaveDataSize2Hook();
	static void __stdcall SaveFileHook();

	static int __cdecl HookedGetString(int param_1);

	static bool OnItemAvailable(void* itemPtr);
	static void OnChunkLoaded();
	static void SetBarriers();
	static void SetParkingPads();
	static void NopInstructions(void* address, size_t size);
	static void TryDisableFourbieTrigger();
	static void TryEditFourbieTrigger(bool enable);
	static void LoadAPSaveFile();
	static int GetSaveDataSize();
	
	static int SaveFile(const char* filename, void* data, int size);
	static void write_json_file(const std::string& filename);
	static void read_json_file(const std::string& filename);
	static bool IsInGame();
	static void KillTy();
	static void EnableLoadButtons();
	static void DisableLoadButtons();
	static void removeCurrawong();
	static void __fastcall DeathHook(void* thisptr, int edx, int state, int source);

	static bool hasRunSetup;

	static void RunLoadSetup(SlotData* slotdata);
	static void SetShopItems(SlotData* slotdata);
	static void SetMissionRequirements(BarrierUnlock unlockType, int mission_goal);
private:
	static uintptr_t bobsCogRequirementArrays[9];
	static uintptr_t OrbRequirementArrays[3];
	static bool doesSaveExist();
};