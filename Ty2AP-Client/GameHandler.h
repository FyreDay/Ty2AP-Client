#pragma once
#include "CheckHandler.h"
#include "APSaveData.h"

typedef int(__cdecl* GetStringFunc)(int id);
extern GetStringFunc originalGetString;

class GameHandler
{
public:
	static void Initialize();
	static void SetupHooks();

	static void __stdcall ItemAvailableUIHook();
	static void __stdcall ItemAvailableLogicHook();
	static void __stdcall ItemAvailableClickHook();
	static void __stdcall ItemAvailableClick2Hook();

	static int __cdecl HookedGetString(int param_1);

	static bool OnItemAvailable(void* itemPtr);
};