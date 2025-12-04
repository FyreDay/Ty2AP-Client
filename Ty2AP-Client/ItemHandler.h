#pragma once
#include "GameHandler.h"
#include "SaveData.h"

class ItemHandler
{
public:
    static void HandleItem(APClient::NetworkItem item);
    
    static void HandleRang(int id);
    static void HandleParkingPad(int id);
    static void CollectItem(int shopId, int itemId);
    static void HandleStoredItems();
    static const char* GetShopItemName(int strId, bool hint);
    static void FillShopItemNames(const std::list<APClient::NetworkItem>& items);
    //static void HandleProgressiveParkingPad();
    static std::queue<APClient::NetworkItem> storedItems;

    static const char* getImagePtr(const std::string& itemname, int flags)
    {
        API::LogPluginMessage("Lookup");
        // lookup by item name
        if (auto it = itemNameToImg.find(itemname); it != itemNameToImg.end())
            return it->second;
        API::LogPluginMessage("Check flags");
        // check flags (priority order)
        if (flags & 1) return "ap_color"; //progressive
        if (flags & 2) return "ap_color"; //useful ap_white
        if (flags & 4) return "ap_color"; //junk ap_black

        // default
        API::LogPluginMessage("Returning default");
        return "ap_color";
    }
private:
    static inline std::unordered_map<std::string, const char*> itemNameToImg = {
        {"PowerStar", "AP_PowerStar"},
        {"Pagie", "AP_Pagie"},

        {"Fire Thunder Egg", "fe_999_icon_thunegg"},
        {"Ice Thunder Egg", "fe_999_icon_thunegg"},
        {"Air Thunder Egg", "fe_999_icon_thunegg"},
        {"Picture Frame", "AP_PictureFrame"},
        {"Golden Cog", "AP_GoldenCog"},


        {"100 Opals", "fe_000_opal"},
        {"200 Opals", "fe_000_opal"},
        {"500 Opals", "fe_000_opal"},
        {"1000 Opals", "fe_000_opal"},
        {"5000 Opals", "fe_000_opal"},
        {"Multirang", "Fe_900_Fe_900_Multirang"},
        {"Flamerang", "Fe_900_Flamerang"},
        {"Frostyrang", "Fe_900_Frostyrang"},
        {"Freezerang", "Fe_900_Freezerang"},
        {"Zappyrang", "Fe_900_Zappyrang"},
        {"Thunderang", "Fe_900_Thunderang"},
        {"Lasharang", "Fe_900_Lasharang"},
        {"Warperang", "Fe_900_Warperang"},
        {"Infrarang", "Fe_900_Infrarang"},
        {"X-Rang", "Fe_900_Xrang"},
        {"Smasharang", "Fe_900_Smasharang"},
        {"Kaboomarang", "Fe_900_Kaboomarang"},
        {"Megarang", "Fe_900_Megarang"},
        {"Omegarang", "Fe_900_Omegarang"},
        {"Deadlyrang", "Fe_900_Deadlyrang"},
        {"Doomerang", "Fe_900_Doomerang"},
        {"Craftyrang", "Fe_900_Craftyrang"},

        {"Progressive Boomerang", "Fe_900_Multirang"},
        {"Progressive Flamerang", "Fe_900_Flamerang"},
        {"Progressive Frostyrang", "Fe_900_Frostyrang"},
        {"Progressive Zappyrang", "Fe_900_Zappyrang"},
        {"Progressive Lasharang", "Fe_900_Lasharang"},
        {"Progressive Infrarang", "Fe_900_Infrarang"},
        {"Progressive Smasharang", "Fe_900_Smasharang"},

        {"Patchy Barriers", "fe_999_tyre"},
        {"Buster Barriers", "fe_999_tyre"},
        {"Fluffy Barriers", "fe_999_tyre"},

        {"Lifter Bunyip Key", "fe_999_lifterbunyip"},
        {"Thermo Bunyip Key", "fe_999_thermobunyip"},
        {"Sub Bunyip Key", "fe_999_sub"},
        {"Progressive Health Paw", "fe_900_GoldPaw"},
    };

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