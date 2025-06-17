#include "pch.h"
#include "SlotData.h"

SlotData::SlotData() {
	requireBosses = true;
	barrierUnlockStyle = BarrierUnlock::OpenWorld;
	cogPrices.push_back(10);
	orbPrices.push_back(3);
}