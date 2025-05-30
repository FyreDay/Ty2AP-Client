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
        return;
    }

    static int progressiveBoomerang(){
        if (!SaveData::GetData()->BoomerangData.GotBoomerang) {
            return 0;
        }
        if (!SaveData::GetData()->BoomerangData.GotMultirang) {
            return 1;
        }
        if (!SaveData::GetData()->BoomerangData.GotMegarang) {
            return 0xe;
        }
        return 0xf;
    };

    static int progressiveFlamerang() {
        if (!SaveData::GetData()->BoomerangData.GotFlamerang) {
            return 2;
        }
        return 3;
    };

    static int progressiveFrostyrang() {
        if (!SaveData::GetData()->BoomerangData.GotFrostyrang) {
            return 4;
        }
        return 5;
    };
    static int progressiveZappyrang() {
        if (!SaveData::GetData()->BoomerangData.GotZappyrang) {
            return 6;
        }
        return 7;
    };

    static int progressiveLasharang() {
        if (!SaveData::GetData()->BoomerangData.GotLasharang) {
            return 8;
        }
        return 9;
    };

    static int progressiveInfrarang() {
        if (!SaveData::GetData()->BoomerangData.GotInfrarang) {
            return 0xa;
        }
        return 0xb;
    };

    static int progressiveSmasharang() {
        if (!SaveData::GetData()->BoomerangData.GotCraftyrang) {
            return 0x14;
        }
        if (!SaveData::GetData()->BoomerangData.GotSmasharang) {
            return 0xc;
        }
        if (!SaveData::GetData()->BoomerangData.GotKaboomarang) {
            return 0xd;
        }
        if (!SaveData::GetData()->BoomerangData.GotDeadlyrang) {
            return 0x10;
        }
        return 0x11;
    };
};