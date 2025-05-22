#include "pch.h"
#include "ItemHandler.h"

std::queue<APClient::NetworkItem> ItemHandler::storedItems;

void ItemHandler::HandleItem(APClient::NetworkItem item)
{
	if (item.item < 0x16) {
		HandleRang(item.item);
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
