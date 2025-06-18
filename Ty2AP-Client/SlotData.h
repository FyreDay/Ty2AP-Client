#pragma once

enum class BarrierUnlock {
	Vanilla = 0,
	Items = 1,
	OpenWorld = 2,
};

class SlotData
{
public:
	SlotData();
	bool requireBosses;
	int missionsToGoal;
	BarrierUnlock barrierUnlockStyle;
	std::vector<int> cogPrices;
	std::vector<int> orbPrices;
};