#include "pch.h"
#include "ItemHandler.h"

std::queue<APClient::NetworkItem> ItemHandler::storedItems;

void ItemHandler::HandleItem(APClient::NetworkItem item)
{
	if (!GameHandler::IsInGame()) {
		storedItems.push(item);
		return;
	}
	Logging::log("lastindex: " + std::to_string(ArchipelagoHandler::customSaveData->pLastReceivedIndex) + " index: " + std::to_string(item.index));
	if (item.index <= ArchipelagoHandler::customSaveData->pLastReceivedIndex) {
		return;
	}

	ArchipelagoHandler::customSaveData->pLastReceivedIndex++;

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
		HandleRang(progressiveZappyrang());
	}
	if (item.item == 0x1a) {
		HandleRang(progressiveLasharang());
	}
	if (item.item == 0x1b) {
		HandleRang(progressiveInfrarang());
	}
	if (item.item == 0x1c) {
		HandleRang(progressiveSmasharang());
	}

	if (item.item == 51) {
		CollectItem(1, 1);
	}
	if (item.item == 52) {
		CollectItem(1, 2);
	}
	
	if (item.item == 59) {
		CollectItem(1, 59);
	}
	if (item.item == 55) {
		CollectItem(4, 55);
	}
	if (item.item == 56) {
		CollectItem(4, 56);
	}
	if (item.item == 57) {
		CollectItem(4, 57);
	}

	if (item.item == 77) {
		CollectItem(1,77);
	}
	if (item.item == 78) {
		CollectItem(1,78);
	}

	if (item.item == 0x20) {
		SaveData::GetData()->CogCollected++;
	}
	if (item.item == 0x21) {
		SaveData::GetData()->OrbCollected++;
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
		int* health = reinterpret_cast<int*>(Core::moduleBase + 0x4BC304);
		int* healthPer = reinterpret_cast<int*>(Core::moduleBase + 0x4BC2FC);
		int* numPaws = reinterpret_cast<int*>(Core::moduleBase + 0x4BC300);
		*health = *healthPer * *numPaws;
	}

	if (item.item >= 3000 && item.item < 5000) {
		HandleParkingPad(item.item);
		if (item.item == 3732) {
			HandleParkingPad(3708);
		}
	}

	if (item.item == 980) {
		std::optional<MissionWrapper> mission980 = SaveData::findMissionByID(980);
		if (mission980.has_value()) {
			Missions::UpdateMissionState((MissionStruct*)mission980.value().address, 5, 0);
		}
	}
	if (item.item == 981) {
		std::optional<MissionWrapper> mission981 = SaveData::findMissionByID(981);
		if (mission981.has_value()) {
			Missions::UpdateMissionState((MissionStruct*)mission981.value().address, 5, 0);
		}
	}
	if (item.item == 982) {
		std::optional<MissionWrapper> mission982 = SaveData::findMissionByID(982);
		if (mission982.has_value()) {
			Missions::UpdateMissionState((MissionStruct*)mission982.value().address, 5, 0);
		}
	}
	std::string filePath = "./Saves/" + ArchipelagoHandler::GetSaveIdentifier();

	GameHandler::write_json_file(filePath + ".json");
}

void ItemHandler::HandleRang(int id)
{
	//clause for boomerang
	if (id == 0x13) {
		id = 0;
	}
	bool* base = &SaveData::GetData()->GotBoomerang;
	base[id] = true;
	//if infrarang or x rang update that ty has them
	if (id == 0x0a) {
		
		CollectItem(2,12);
	}
	if (id == 0x0b) {
		CollectItem(3,20);
		CollectItem(2, 12);
	}
	if (id == 0x04) {
		
	}
	if (id == 0x05) {
		CollectItem(3, 5);
		CollectItem(2, 4);
	}
	if (id == 0x02) {
		
	}
	if (id == 0x03) {
		CollectItem(3, 3);
		CollectItem(2, 2);
	}
}

void ItemHandler::HandleParkingPad(int id)
{
	ArchipelagoHandler::customSaveData->UnlockedParkingPads.push_back(id);
	//updates parkingpads if its loaded nned to queue for next frame
	GameHandler::OnChunkLoaded();
}

void ItemHandler::CollectItem(int shopId, int itemId)
{
	LinkedList<ItemWrapper> items = SaveData::ItemList(shopId);
	std::optional<ItemWrapper> item = SaveData::findItemByID(items, itemId);
	if (item.has_value()) {
		Logging::log("item has value: " + std::to_string(itemId) + "  " + std::to_string(item.value().address));
		item.value().setPuchusedStatus(true);
	}
	else {
		Logging::log("No Item with id " + std::to_string(itemId));
	}
}

void ItemHandler::HandleStoredItems()
{
	while (!storedItems.empty() && GameHandler::IsInGame()) {
		HandleItem(storedItems.front());
		storedItems.pop();
	}
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