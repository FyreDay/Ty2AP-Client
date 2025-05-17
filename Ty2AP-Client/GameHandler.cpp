#include "pch.h"
#include "GameHandler.h"

void GameHandler::Initialize()
{
	MH_Uninitialize();
	MH_Initialize();

	CheckHandler::SetupHooks();

	MH_EnableHook(MH_ALL_HOOKS);
}