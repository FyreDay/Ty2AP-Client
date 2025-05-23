#pragma once



class APSaveData
{
public:
	static bool hasBoughtItem(int id) {
		return ItemMap[id];
	}
	static void updateBoughtItem(int id, bool bought) {
		ItemMap[id] = bought;
	}
	static inline std::list<int> UnlockedParkingPads = { 3736 };
	static inline const std::list<int> AllParkingPads = { 3736,3689 };
	static inline std::map<int, bool> ItemMap = {
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