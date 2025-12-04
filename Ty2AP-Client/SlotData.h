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
	bool deathlink;
	bool requireBosses;
	bool progressiveRangs;
	bool skipCurrawong;
	int missionsToGoal;
	BarrierUnlock barrierUnlockStyle;
	std::vector<int> rangPrices;
	std::vector<int> slyPrices;
	std::vector<int> traderBobPrices;
	std::vector<int> cogPrices;
	std::vector<int> orbPrices;
};