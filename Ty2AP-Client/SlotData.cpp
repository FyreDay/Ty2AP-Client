#include "pch.h"
#include "SlotData.h"

SlotData::SlotData() {
	requireBosses = true;
	barrierUnlockStyle = BarrierUnlock::OpenWorld;
	rangPrices.push_back(8);
	slyPrices.push_back(11);
	traderBobPrices.push_back(5);
	cogPrices.push_back(10);
	orbPrices.push_back(3);
}