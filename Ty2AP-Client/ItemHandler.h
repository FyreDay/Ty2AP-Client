#pragma once
#include "GameHandler.h"
#include "SaveData.h"
#include "Item.h"
#include "Shop.h"

class ItemHandler
{
public:
    static void HandleItem(APClient::NetworkItem item);
    static void HandleRang(APItem id);
    static void HandleParkingPad(int id);
    static void CollectItem(int shopId, int itemId);
    static void HandleStoredItems();
    static const char* GetShopItemName(int strId, bool hint);
    static void FillShopItemNames(const std::list<APClient::NetworkItem>& items);

    static std::queue<APClient::NetworkItem> storedItems;

    static const char* getImagePtr(const std::string& itemname, int flags)
    {
        // lookup by item name
        if (auto it = itemNameToImg.find(itemname); it != itemNameToImg.end())
            return it->second;

        // check flags (priority order)
        if (flags & 1) return "ap_color"; //progressive
        if (flags & 2) return "ap_color"; //useful ap_white
        if (flags & 4) return "ap_color"; //junk ap_black

        // default
        return "ap_color";
    }
private:
    static inline std::unordered_map<std::string, const char*> itemNameToImg = {
        {"Power Star", "AP_PowerStar"},
        {"Pagie", "AP_Pagie"},
        {"Egg", "AP_Egg"},
        {"Emblem", "AP_Emblem"},
        {"Gold Bolt", "AP_GoldBolt"},
        {"Golden Banana", "AP_GoldenBanana"},
        {"Golden Spatula", "AP_GoldenSpatula"},
        {"Jiggy", "AP_Jiggy"},
        {"Orb", "AP_Orb"},
        {"Platinum Bolt", "AP_PlatinumBolt"},
        {"Platinum Cog", "AP_PlatinumCog"},
        {"Precursor Orb", "AP_PrecursorOrb"},
        {"Shine Sprite", "AP_ShineSprite"},
        {"Time Piece", "AP_TimePiece"},
        {"Titanium Bolt", "AP_TitaniumBolt"},
        {"Strawberry", "AP_Strawberry"},
        {"100 Opals", "fe_000_opal"},
        {"200 Opals", "fe_000_opal"},
        {"500 Opals", "fe_000_opal"},
        {"1000 Opals", "fe_000_opal"},
        {"5000 Opals", "fe_000_opal"},
        {"Multirang", "Fe_900_Multirang"},
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
        {"Bilby", "fe_000_bilby"},
        {"Kromium Orb", "fe_000_kromeorb"},
        {"Platinum Cog", "fe_000_platcog"},
        {"Picture Frame", "fe_000_picframe"},
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

    static APItem progressiveBoomerang(){
        if (!SaveData::GetData()->GotBoomerang) 
            return APItem::BOOMERANG;
        if (!SaveData::GetData()->GotMultirang) 
            return APItem::MULTIRANG;
        if (!SaveData::GetData()->GotMegarang)
            return APItem::MEGARANG;
        return APItem::OMEGARANG;
    };

    static APItem progressiveFlamerang() {
        if (!SaveData::GetData()->GotFlamerang)
            return APItem::FLAMERANG;
        return APItem::LAVARANG;
    };

    static APItem progressiveFrostyrang() {
        if (!SaveData::GetData()->GotFrostyrang)
            return APItem::FROSTYRANG;
        return APItem::FREEZERANG;
    };

    static APItem progressiveZappyrang() {
        if (!SaveData::GetData()->GotZappyrang)
            return APItem::ZAPPYRANG;
        return APItem::THUNDERANG;
    };

    static APItem progressiveLasharang() {
        if (!SaveData::GetData()->GotLasharang) 
            return APItem::LASHARANG;
        return APItem::WARPERANG;
    };

    static APItem progressiveInfrarang() {
        if (!SaveData::GetData()->GotInfrarang)
            return APItem::INFRARANG;
        return APItem::XRANG;
    };

    static APItem progressiveSmasharang() {
        if (!SaveData::GetData()->GotCraftyrang)
            return APItem::CRAFTYRANG;
        if (!SaveData::GetData()->GotSmasharang)
            return APItem::SMASHARANG;
        if (!SaveData::GetData()->GotKaboomarang)
            return APItem::KABOOMERANG;
        if (!SaveData::GetData()->GotDeadlyrang)
            return APItem::DEADLYRANG;
        return APItem::DOOMERANG;
    };
};