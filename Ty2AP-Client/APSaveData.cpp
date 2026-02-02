#include "pch.h"
#include "APSaveData.h"

APSaveData::APSaveData() {
	UnlockedParkingPads = { };
	UnlockedBarriers = { };
}

void APSaveData::CheckGoal()
{
	missionsCompleted = ArchipelagoHandler::CountLocationsChecked(missionLocs);

	if (missionsCompleted < ArchipelagoHandler::slotdata->missionsToGoal) return;

	LoggerWindow::Log("All required missions completed! Check back at Bush Rescue HQ.");
	auto mission = SaveData::findMissionByID(Mission::SEE_JULIUS);
	mission->numberPreconditionMissionNeeded = 0;
	Missions::UpdateMissionState(mission, 1, 0);
}
