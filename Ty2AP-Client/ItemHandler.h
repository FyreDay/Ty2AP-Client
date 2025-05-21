#pragma once
#include "apclient.hpp"
#include "ArchipelagoHandler.h"

class ItemHandler
{
public:
    static void HandleItem(APClient::NetworkItem item);
    
    static void HandleProgressiveRang();
    static void HandleParkingPad(int id);
    //static void HandleProgressiveParkingPad();
};