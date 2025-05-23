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
	uintptr_t parkingpad = MKObject::GetMKObject(id);
}

void ItemHandler::CollectItem(int offsetfromfirstitem)
{
	uintptr_t rawAddress = reinterpret_cast<uintptr_t>(&SaveData::GetData()->FirstItem);
	uintptr_t newAddress = rawAddress + offsetfromfirstitem + 0x10;
	bool* boolPtr = reinterpret_cast<bool*>(newAddress);
	*boolPtr = true;
}


