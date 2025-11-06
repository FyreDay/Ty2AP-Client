#pragma once



class APSaveData
{
public:

	APSaveData();
	bool hasBoughtItem(int id) {
		return ItemMap[id];
	}
	void updateBoughtItem(int id, bool bought) {
		ItemMap[id] = bought;
	}
	int pLastReceivedIndex = -1;
	int cogCount = 0;
	int orbCount = 0;
	std::list<int> UnlockedParkingPads = {
	};
	const std::list<int> AllParkingPads = 
	{ 
		3736,
		3689,
		3688,
		4092,
		3692,
		3306,
		3285,
		3287,
		3712,
		3735,
		3694,
		3693,
		3292,
		4130,
		3685,
		3687,
		3983,
		3244,
		3714,
		3702,
		3732,
		3713,
		3284,
		3709,
		3710,
		3711,
		4035,
		3300,
		3951,
		3686,
		3733,
		3967,
		3690,
		3691,
		3954,
		3277,
		4046,
		3972
	};
	std::map<int, bool> ItemMap = {
	{8, false}, //True is purchused
	{9, false},
	{10, false},
	{11, false},
	{12, false},
	{13, false},
	{14, false},
	{26, false},
	{15, false},
	{16, false},
	{17, false},
	{18, false},
	{19, false},
	{20, false},
	{21, false},
	{22, false},
	{23, false},
	{24, false},
	{25, false},
	{1, false},
	{2, false},
	{59, false},
	{77, false},
	{78, false},
	{79, false},
	{80, false},
	{81, false},
	{82, false},
	{83, false},
	{84, false},
	{85, false},
	{86, false},
	{87, false},
	{88, false},
	{5, false},
	{6, false},
	{7, false},
	};
};