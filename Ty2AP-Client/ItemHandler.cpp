#include "pch.h"
#include "ItemHandler.h"

std::queue<APClient::NetworkItem> ItemHandler::storedItems;

void ItemHandler::HandleItem(APClient::NetworkItem item)
{
	if (!GameHandler::IsInGame()) {
		storedItems.push(item);
		return;
	}
	API::LogPluginMessage("lastindex: " + std::to_string(ArchipelagoHandler::customSaveData->pLastReceivedIndex) + " index: " + std::to_string(item.index));
	if (item.index <= ArchipelagoHandler::customSaveData->pLastReceivedIndex) {
		return;
	}

	ArchipelagoHandler::customSaveData->pLastReceivedIndex++;

	if (item.item < (int)APItem::PROGRESSIVE_BOOMERANG)
		HandleRang((APItem)item.item);

	if ((APItem)item.item == APItem::PROGRESSIVE_BOOMERANG) 
		HandleRang(progressiveBoomerang());

	if ((APItem)item.item == APItem::PROGRESSIVE_FLAMERANG) 
		HandleRang(progressiveFlamerang());

	if ((APItem)item.item == APItem::PROGRESSIVE_FROSTYRANG) 
		HandleRang(progressiveFrostyrang());

	if ((APItem)item.item == APItem::PROGRESSIVE_ZAPPYRANG) 
		HandleRang(progressiveZappyrang());

	if ((APItem)item.item == APItem::PROGRESSIVE_LASHARANG) 
		HandleRang(progressiveLasharang());

	if ((APItem)item.item == APItem::PROGRESSIVE_INFRARANG)
		HandleRang(progressiveInfrarang());

	if ((APItem)item.item == APItem::PROGRESSIVE_SMASHARANG)
		HandleRang(progressiveSmasharang());

	if ((APItem)item.item == APItem::LIFTER_KEY) {
		ArchipelagoHandler::customSaveData->hasLifterKey = true;
		CollectItem(Shop::TRADER_BOBS, (int)Item::LIFTER_KEY);
	}

	if ((APItem)item.item == APItem::THERMO_KEY) {
		ArchipelagoHandler::customSaveData->hasThermoKey = true;
		CollectItem(Shop::TRADER_BOBS, (int)Item::THERMO_KEY);
	} 
	
	if ((APItem)item.item == APItem::SUB_KEY) {
		ArchipelagoHandler::customSaveData->hasSubKey = true;
		CollectItem(Shop::TRADER_BOBS, (int)Item::SUB_KEY);
	}

	if ((APItem)item.item == APItem::MAP_BILBY) {
		ArchipelagoHandler::customSaveData->hasBilbyMap;
		CollectItem(Shop::MADAME_MOPOKES, (int)Item::MAP_BILBY);
	}

	if ((APItem)item.item == APItem::MAP_COGS) {
		ArchipelagoHandler::customSaveData->hasCogMap;
		CollectItem(Shop::MADAME_MOPOKES, (int)Item::MAP_COGS);
	}

	if ((APItem)item.item == APItem::MAP_STEVE) {
		ArchipelagoHandler::customSaveData->hasSteveMap;
		CollectItem(Shop::MADAME_MOPOKES, (int)Item::MAP_STEVE);
	}

	if ((APItem)item.item == APItem::PROGRESSIVE_PAW) {
		if (!ArchipelagoHandler::customSaveData->hasGoldPaw) {
			CollectItem(Shop::TRADER_BOBS, (int)Item::GOLD_PAW);
			ArchipelagoHandler::customSaveData->hasGoldPaw = true;
		}
		else {
			CollectItem(Shop::TRADER_BOBS, (int)Item::PLATINUM_PAW);
		}
	}

	if ((APItem)item.item == APItem::PLATINUM_COG) {
		ArchipelagoHandler::customSaveData->cogCount++;
		SaveData::GetData()->CogCollected++;
	}

	if ((APItem)item.item == APItem::KROMIUM_ORB) {
		ArchipelagoHandler::customSaveData->orbCount++;
		SaveData::GetData()->OrbCollected++;
	}

	if ((APItem)item.item == APItem::OPALS_100)
		AddOpals(100);

	if ((APItem)item.item == APItem::OPALS_200)
		AddOpals(200);

	if ((APItem)item.item == APItem::OPALS_500)
		AddOpals(500);

	if ((APItem)item.item == APItem::OPALS_1000)
		AddOpals(1000);

	if ((APItem)item.item == APItem::OPALS_5000)
		AddOpals(5000);

	if ((APItem)item.item == APItem::FULL_PIE) {
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

	if ((APItem)item.item == APItem::PATCHY_BARRIERS) {
		ArchipelagoHandler::customSaveData->UnlockedBarriers.push_back((int)Item::PATCHY_BARRIERS);
		GameHandler::OnChunkLoaded();
	}

	if ((APItem)item.item == APItem::FLUFFY_BARRIERS) {
		ArchipelagoHandler::customSaveData->UnlockedBarriers.push_back((int)Item::FLUFFY_BARRIERS);
		GameHandler::OnChunkLoaded();
	}

	if ((APItem)item.item == APItem::BUSTER_BARRIERS) {
		ArchipelagoHandler::customSaveData->UnlockedBarriers.push_back((int)Item::BUSTER_BARRIERS);
		GameHandler::OnChunkLoaded();
	}

	std::string filePath = "./Saves/" + ArchipelagoHandler::GetSaveIdentifier();

	GameHandler::write_json_file(filePath + ".json");
}

void ItemHandler::HandleRang(APItem item)
{
	//clause for boomerang
	if (item == APItem::BOOMERANG) {
		item = APItem::NONE;
	}

	bool* base = &SaveData::GetData()->GotBoomerang;
	base[(int)item] = true;

	if (item == APItem::INFRARANG) {
		CollectItem(Shop::RANG_SHOP, (int)Item::INFRARANG);
	}
	if (item == APItem::XRANG) {
		CollectItem(Shop::SLYS_SHACK, (int)Item::XRANG);
		CollectItem(Shop::RANG_SHOP, (int)Item::INFRARANG);
	}

	if (item == APItem::FROSTYRANG) {
		CollectItem(Shop::RANG_SHOP, (int)Item::FROSTYRANG);
	}
	if (item == APItem::FREEZERANG) {
		CollectItem(Shop::SLYS_SHACK, (int)Item::FREEZERANG);
		CollectItem(Shop::RANG_SHOP, (int)Item::FROSTYRANG);
	}

	if (item == APItem::FLAMERANG) {
		CollectItem(Shop::RANG_SHOP, (int)Item::FLAMERANG);
	}
	if (item == APItem::LAVARANG) {
		CollectItem(Shop::SLYS_SHACK, (int)Item::LAVARANG);
		CollectItem(Shop::RANG_SHOP, (int)Item::FLAMERANG);
	}
}

void ItemHandler::HandleParkingPad(int id)
{
	ArchipelagoHandler::customSaveData->UnlockedParkingPads.push_back(id);
	GameHandler::OnChunkLoaded();
}

void ItemHandler::CollectItem(int shopId, int itemId)
{
	LinkedList<ItemStruct> items = SaveData::GetShopItemList(shopId);
	auto item = SaveData::findItemByID(items, itemId);
	if (item) {
		API::LogPluginMessage("item has value: " + std::to_string(itemId));
		item->purchased = true;
	}
	else
		API::LogPluginMessage("No Item with id " + std::to_string(itemId));
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

static std::map<int, std::string> strIDtoImgText = {};
static std::mutex strIDtoImgTextMutex;

struct itemStrings {
	int titleId;
	int descId;
};

static const std::map <int, itemStrings> shop1Ids =
{ { 1, {550,551} }, { 2, {552,553} }, { 59, {666,667} },{77, {702,703} },{78, {704,705} },{79, {1117,1118} },{80, {1119,1120} },{81, {1121,1122} },
	{82, {1123,1124} },{83, {1125,1126} },{84, {1127,1128} },{85, {1129,1130} },{86, {1131,1132} }, { 87, {1133,1134} }, { 88, {1135,1136} } };
static const std::map <int, itemStrings> shop2Ids = 
{ {8, {564,212} },{9, {566,214} },{18, {196,216} },{11, {570,218} },{12, {572,220} },{13, {574,222} },{14, {576,224} },{26, {1190,1191} } };
static const std::map <int, itemStrings> shop3Ids = 
{ {15, {191,211} },{16, {193,213} },{17, {195,215} },{10, {568,217} },{19, {199,219} },{20, {201,221} },{21, {203,223} },{22, {205,225} },
	{23, {206,226} },{24, {207,227} },{25, {1188,1189} } };
static const std::map <int, itemStrings> shop4Ids = { {5, {558,559} },{6, {560,561} },{7, {562,563} } };


std::optional<int> getItemIdFromString(int stringId) {
	const std::map<int, itemStrings>* shops[] = { &shop1Ids, &shop2Ids, &shop3Ids, &shop4Ids };

	for (auto shop : shops) {
		for (const auto& pair : *shop) {
			if (pair.second.titleId == stringId || pair.second.descId == stringId) {
				return pair.first; // return the item ID
			}
		}
	}

	return std::nullopt; // not found
}

std::list<int64_t> scouteditems;

static const char* unknown = "Unknown AP Item";

const char* ItemHandler::GetShopItemName(int strId, bool hint) {
	auto itemIdOpt = getItemIdFromString(strId);
	if (itemIdOpt.has_value()) {
		int64_t itemId = static_cast<int64_t>(itemIdOpt.value());
		if (std::find(scouteditems.begin(), scouteditems.end(), itemId) == scouteditems.end()) {
			std::list<int64_t> newList;
			newList.push_back(itemId);
			ArchipelagoHandler::ScoutLocations(newList, hint);

			if (hint) {
				scouteditems.push_back(itemId);
			}
		}
	}
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
		ItemStruct* shopItem = SaveData::findItemByID((int)item.location);
		if (shopItem) {
			shopItem->ShopIconNameString = const_cast<char*>(ItemHandler::getImagePtr(ArchipelagoHandler::GetItemName(item.item, item.player), item.flags));
		}
	}
}