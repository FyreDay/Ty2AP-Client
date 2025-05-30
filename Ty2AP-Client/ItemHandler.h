#pragma once
#include "GameHandler.h"
#include "SaveData.h"

class ItemHandler
{
public:
    static void HandleItem(APClient::NetworkItem item);
    
    static void HandleRang(int id);
    static void HandleParkingPad(int id);
    static void CollectItem(int offsetfromfirstitem);
    static void HandleStoredItems();
    static const char* GetShopItemName(int strId);
    static void FillShopItemNames(const std::list<APClient::NetworkItem>& items);
    //static void HandleProgressiveParkingPad();
    static std::queue<APClient::NetworkItem> storedItems;
private:
    static void AddOpals(int amount) {
        using Addopals = uint32_t(__thiscall*)(uintptr_t thisPtr, uintptr_t objPtr);
        Addopals addopals = reinterpret_cast<Addopals>((uintptr_t)API::Get()->param()->TyHModule + 0x00119fb0);
        uintptr_t thisPtr = (uintptr_t)API::Get()->param()->TyHModule + 0x4EB580;

        addopals(thisPtr, amount);
        int* uiPtr = (int*)(Core::moduleBase + 0x4EB588);
        *uiPtr += amount;

        int* visualCount = (int*)(*(uintptr_t*)(Core::moduleBase + 0x4EEB9C) + 0x510);
        *visualCount += amount;
        return;
    }

    static int progressiveBoomerang(){
        if (!SaveData::GetData()->GotBoomerang) {
            return 0;
        }
        if (!SaveData::GetData()->GotMultirang) {
            return 1;
        }
        if (!SaveData::GetData()->GotMegarang) {
            return 0xe;
        }
        return 0xf;
    };

    static int progressiveFlamerang() {
        if (!SaveData::GetData()->GotFlamerang) {
            return 2;
        }
        return 3;
    };

    static int progressiveFrostyrang() {
        if (!SaveData::GetData()->GotFrostyrang) {
            return 4;
        }
        return 5;
    };
    static int progressiveZappyrang() {
        if (!SaveData::GetData()->GotZappyrang) {
            return 6;
        }
        return 7;
    };

    static int progressiveLasharang() {
        if (!SaveData::GetData()->GotLasharang) {
            return 8;
        }
        return 9;
    };

    static int progressiveInfrarang() {
        if (!SaveData::GetData()->GotInfrarang) {
            return 0xa;
        }
        return 0xb;
    };

    static int progressiveSmasharang() {
        if (!SaveData::GetData()->GotCraftyrang) {
            return 0x14;
        }
        if (!SaveData::GetData()->GotSmasharang) {
            return 0xc;
        }
        if (!SaveData::GetData()->GotKaboomarang) {
            return 0xd;
        }
        if (!SaveData::GetData()->GotDeadlyrang) {
            return 0x10;
        }
        return 0x11;
    };
};