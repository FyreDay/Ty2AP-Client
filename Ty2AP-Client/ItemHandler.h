#pragma once
#include "ArchipelagoHandler.h"
#include "SaveData.h"

class ItemHandler
{
public:
    static void HandleItem(APClient::NetworkItem item);
    
    static void HandleRang(int id);
    template<typename T, std::size_t N>
    static void HandleProgressiveRang(const std::array<T, N>& arr) {
        
    }
    static void HandleParkingPad(int id);
    //static void HandleProgressiveParkingPad();
    static std::queue<APClient::NetworkItem> storedItems;
private:
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
        if (!SaveData::GetData()->BoomerangData.GotOmegarang) {
            return 0xf;
        }
    };

    static int progressiveBoomerang() {
        if (!SaveData::GetData()->BoomerangData.GotFlamerang) {
            return 0;
        }
        if (!SaveData::GetData()->BoomerangData.GotLavarang) {
            return 1;
        }
        if (!SaveData::GetData()->BoomerangData.GotMegarang) {
            return 0xe;
        }
        if (!SaveData::GetData()->BoomerangData.GotOmegarang) {
            return 0xf;
        }
    };

    static inline const std::array<int, 2> progressiveFlamerang{
        2,
        3
    };

    static inline const std::array<int, 2> progressiveFrostyrang{
        4,
        5
    };

    static inline const std::array<int, 2> progressiveZappyrang{
        6,
        7
    };

    static inline const std::array<int, 2> progressiveLasharang{
        8,
        9
    };
    static inline const std::array<int, 2> progressiveInfrarang{
        0xa,
        0xb
    };

    static inline const std::array<int, 5> progressiveSmasharang{
        0x14,
        0xc,
        0xd,
        0x10,
        0x11
    };
};