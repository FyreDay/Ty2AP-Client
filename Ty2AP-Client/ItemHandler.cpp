#include "pch.h"
#include "ItemHandler.h"

std::queue<APClient::NetworkItem> ItemHandler::storedItems;

void ItemHandler::HandleItem(APClient::NetworkItem item)
{
	if (item.item < 0x16) {
		HandleRang(item.item);
	}
	if (item.item = 0x16) {
		HandleRang(progressiveBoomerang());
	}
	if (item.item = 0x17) {
		HandleRang(progressiveFlamerang());
	}
	if (item.item = 0x18) {
		HandleRang(progressiveFrostyrang());
	}
	if (item.item = 0x19) {
		HandleRang(progressiveLasharang());
	}
	if (item.item = 0x1a) {
		HandleRang(progressiveInfrarang());
	}
	if (item.item = 0x1b) {
		HandleRang(progressiveSmasharang());
	}\

	if (item.item = 0x20) {
		SaveData::GetData()->CollectableData.Cogs.Collected++;
	}
	if (item.item = 0x21) {
		SaveData::GetData()->CollectableData.Orbs.Collected++;
	}
	if (item.item = 0x22) {
		AddOpals(1);
	}
	if (item.item = 0x23) {
		AddOpals(10);
	}
	if (item.item = 0x24) {
		AddOpals(25);
	}
	if (item.item = 0x25) {
		AddOpals(100);
	}
	if (item.item = 0x26) {
		AddOpals(200);
	}
	if (item.item = 0x27) {
		//heal
	}
	if (item.item >= 3000 && item.item < 4000) {
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
}

void ItemHandler::HandleParkingPad(int id)
{
	uintptr_t parkingpad = MKObject::GetMKObject(id);
}
