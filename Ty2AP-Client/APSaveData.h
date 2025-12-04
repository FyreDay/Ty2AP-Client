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
	std::list<int> UnlockedBarriers = {
	};
	const std::list<int> AllParkingPads = 
	{ 
		3736, // Burramudgee Town
		3689, // Min Min Plains
		3688, // Freeway Training Grounds
		4092, // Beach Training Grounds
		3692, // Dennis Freeway
		3306, // Muddy Bottom
		3285, // Oil Rig
		3287, // Wobbygon Bay
		3712, // Hearty Beach
		3735, // MountBoom End
		3694, // MountBoom Start
		3693, // Frill Neck Forest
		3292, // Old Stony Creek
		4130, // Camping
		3685, // Outback Oasis
		3687, // Refinery Run
		3983, // Fire Fight
		3244, // Sly
		3714, // Outback Dash
		3702, // Truck Tragedy
		3732, // Truck Stop
		3713, // Never Never Road
		3284, // Plutonium Panic
		3709, // 50 Foot Squeaver
		3710, // Never Never
		3711, // Lava Falls Race
		4035, // Min Min Mining
		3300, // Turbo Track
		3951, // Patchy
		3686, // Lake Burramudgee
		3733, // Bush Fire
		3967, // Sulphur Rocks
		3690, // King Squeaver
		3691, // Fluffy
		3954, // Faire Dinkum
		3277, // Wetlands
		4046, // Dusty Burrows
		3972, // Ripper Nipper
		3708, // Truck Stop 2 - Electric Boogaloo
	};
	std::map<int, bool> ItemMap = {
		{8, false}, //True is purchased
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