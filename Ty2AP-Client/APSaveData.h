#pragma once



class APSaveData
{
public:

	APSaveData();
	bool hasBoughtItem(int id) {
		return !ItemMap[id];
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
	{8, true}, //True is can purchuse
	{9, true},
	{10, true},
	{11, true},
	{12, true},
	{13, true},
	{14, true},
	{26, true},
	{15, true},
	{16, true},
	{17, true},
	{18, true},
	{19, true},
	{20, true},
	{21, true},
	{22, true},
	{23, true},
	{24, true},
	{25, true},
	{1, true},
	{2, true},
	{59, true},
	{77, true},
	{78, true},
	{79, true},
	{80, true},
	{81, true},
	{82, true},
	{83, true},
	{84, true},
	{85, true},
	{86, true},
	{87, true},
	{88, true},
	{5, true},
	{6, true},
	{7, true},
	};
};