#include "pch.h"
#include "ItemHandler.h"

std::queue<APClient::NetworkItem> ItemHandler::storedItems;

void ItemHandler::HandleItem(APClient::NetworkItem item)
{
	if (item.item < 0x16) {
		HandleRang(item.item);
	}
	if (item.item == 0x16) {
		HandleRang(progressiveBoomerang());
	}
	if (item.item == 0x17) {
		HandleRang(progressiveFlamerang());
	}
	if (item.item == 0x18) {
		HandleRang(progressiveFrostyrang());
	}
	if (item.item == 0x19) {
		HandleRang(progressiveLasharang());
	}
	if (item.item == 0x1a) {
		HandleRang(progressiveInfrarang());
	}
	if (item.item == 0x1b) {
		HandleRang(progressiveSmasharang());
	}

	if (item.item == 51) {
		CollectItem(0);
	}
	if (item.item == 52) {
		CollectItem(0x50);
	}
	
	if (item.item == 59) {
		CollectItem(0x7D0);
	}
	if (item.item == 55) {
		CollectItem(0xa0);
	}
	if (item.item == 56) {
		CollectItem(0xf0);
	}
	if (item.item == 57) {
		CollectItem(0x14);
	}

	if (item.item == 77) {
		CollectItem(0xD25);
	}
	if (item.item == 78) {
		CollectItem(0xd2a);
	}

	if (item.item == 0x20) {
		SaveData::GetData()->CollectableData.Cogs.Collected++;
	}
	if (item.item == 0x21) {
		SaveData::GetData()->CollectableData.Orbs.Collected++;
	}
	if (item.item == 0x22) {
		AddOpals(1);
	}
	if (item.item == 0x23) {
		AddOpals(10);
	}
	if (item.item == 0x24) {
		AddOpals(25);
	}
	if (item.item == 0x25) {
		AddOpals(100);
	}
	if (item.item == 0x26) {
		AddOpals(200);
	}
	if (item.item == 0x27) {
		//heal
	}
	if (item.item == 51) {

	}

	if (item.item >= 3000 && item.item < 5000) {
		HandleParkingPad(item.item);
	}
}

void ItemHandler::HandleRang(int id)
{
	//clause for boomerang
	if (id == 0x13) {
		id = 0;
	}
	bool* base = &SaveData::GetData()->BoomerangData.GotBoomerang;
	base[id] = true;
	//if infrarang or x rang update that ty has them
	if (id == 0x0a) {
		
		CollectItem(0x2D0);
	}
	if (id == 0x0b) {
		CollectItem(0x2D0 + 0x280);
	}
}

void ItemHandler::HandleParkingPad(int id)
{
	APSaveData::UnlockedParkingPads.push_back(id);
	//updates parkingpads if its loaded
	if (GameHandler::IsInGame()) {
		GameHandler::OnChunkLoaded();
	}
}

void ItemHandler::CollectItem(int offsetfromfirstitem)
{
	uintptr_t rawAddress = reinterpret_cast<uintptr_t>(&SaveData::GetData()->FirstItem);
	uintptr_t newAddress = rawAddress + offsetfromfirstitem + 0x10;
	bool* boolPtr = reinterpret_cast<bool*>(newAddress);
	*boolPtr = true;
}

static std::map<int, std::string> strIDtoTitleText = {};
static std::mutex strIDtoTitleTextMutex;

static std::map<int, std::string> strIDtoDescText = {};
static std::mutex strIDtoDescTextMutex;

struct itemStrings {
	int titleId;
	int descId;
};

static const std::map <int, itemStrings> shop1Ids =
{ { 1, {550,551} }, { 2, {552,553} }, { 59, {666,667} },{77, {702,703} },{78, {704,705} },{79, {1117,1118} },{80, {1119,1120} },{81, {1121,1122} },
	{82, {1123,1124} },{83, {1125,1126} },{84, {1127,1128} },{85, {1129,1130} },{86, {1131,1132} }, { 87, {1133,1134} }, { 88, {1135,1136} } };
static const std::map <int, itemStrings> shop2Ids = 
{ {8, {564,212} },{9, {566,214} },{10, {568,217} },{11, {570,218} },{12, {572,220} },{13, {574,222} },{14, {576,224} },{26, {1190,1191} } };
static const std::map <int, itemStrings> shop3Ids = 
{ {15, {191,211} },{16, {193,213} },{17, {195,215} },{18, {196,216} },{19, {199,219} },{20, {201,221} },{21, {203,223} },{22, {205,225} },
	{23, {206,226} },{24, {207,227} },{25, {1188,1189} } };
static const std::map <int, itemStrings> shop4Ids = { {5, {558,559} },{6, {560,561} },{7, {562,563} } };
static bool shop1Scouted = false;
static bool shop2Scouted = false;
static bool shop3Scouted = false;
static bool shop4Scouted = false;

static const char* unknown = "Unknown AP Item";

const char* ItemHandler::GetShopItemName(int strId) {
	{
		std::lock_guard<std::mutex> lock(strIDtoTitleTextMutex);
		auto it = strIDtoTitleText.find(strId);
		if (it != strIDtoTitleText.end()) {
			return it->second.c_str();
		}
	}
	{
		std::lock_guard<std::mutex> lock(strIDtoDescTextMutex);
		auto it = strIDtoDescText.find(strId);
		if (it != strIDtoDescText.end()) {
			return it->second.c_str();
		}
	}
	auto checkAndScoutOnce = [&](const std::map<int, itemStrings>& shopMap, bool& hasScouted) -> bool {
		for (const auto& [itemId, strings] : shopMap) {
			if (strings.titleId == strId || strings.descId == strId) {
				if (!hasScouted) {
					std::list<int64_t> itemList;
					for (const auto& [id, _] : shopMap) {
						itemList.push_back(id);
					}
					ArchipelagoHandler::ScoutLocations(itemList, 1);
					hasScouted = true;
				}
				return true;
			}
		}
		return false;
	};

	if (checkAndScoutOnce(shop1Ids, shop1Scouted) ||
		checkAndScoutOnce(shop2Ids, shop2Scouted) ||
		checkAndScoutOnce(shop3Ids, shop3Scouted) ||
		checkAndScoutOnce(shop4Ids, shop4Scouted)) {
		return unknown;
	}

	return nullptr;
}

void ItemHandler::FillShopItemNames(const std::list<APClient::NetworkItem>& items) {
	for (const APClient::NetworkItem& item : items) {
		itemStrings* strs = nullptr;

		auto findStrings = [&](const std::map<int, itemStrings>& shopMap) -> itemStrings* {
			auto it = shopMap.find(item.location);
			return (it != shopMap.end()) ? const_cast<itemStrings*>(&it->second) : nullptr;
			};

		if (!(strs = findStrings(shop1Ids)) &&
			!(strs = findStrings(shop2Ids)) &&
			!(strs = findStrings(shop3Ids)) &&
			!(strs = findStrings(shop4Ids))) {
			continue; // Skip if not a known AP shop item
		}

		{
			std::lock_guard<std::mutex> lock(strIDtoTitleTextMutex);
			strIDtoTitleText[strs->titleId] = ArchipelagoHandler::GetItemName(item.item, item.player);
		}
		{
			std::lock_guard<std::mutex> lock(strIDtoDescTextMutex);
			strIDtoDescText[strs->descId] = ArchipelagoHandler::GetItemDesc(item.player);
		}
	}
}

//x = 10;
//y = 7;
//z = y;
//y = x;
//x = z
//
//cout << x << y
//
//
//r=2
//u=r
//
//
//cout << 2+2+2