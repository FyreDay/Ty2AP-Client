#pragma once
#include "gui.h"
#include "InfoWindow.h"
#include "MinHook.h"
#include "APSaveData.h"



class CheckHandler
{
public:
	static void __stdcall CollectCollectibleHook();
	static void OnCollectCollectible(int id, int type);

    static void __stdcall CompleteMissionHook();
    static void OnCompleteMission(void* mission, int status);

    static void __stdcall PurchaseItemHook();
    static void OnBuyItem(void* item);

	static void SetupHooks();

    struct Collectible {
        int level = -1;
        std::string name;
    };

    static inline const std::array<Collectible, 6> collectibles{
        Collectible{0, "Platinum Cog"},
        Collectible{1, "Bilby"},
        Collectible{2, "Frame"},
        Collectible{3, "Kromium Orb"},
        Collectible{4, "Steve"},
        Collectible{5, "Frill"},
    };
};