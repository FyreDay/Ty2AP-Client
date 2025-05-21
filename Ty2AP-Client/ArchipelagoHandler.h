#pragma once
#include "nlohmann/json.hpp"
#include "LoginWindow.h"

class LoginWindow;
#define GAME_NAME "Ty the Tasmanian Tiger 2"
#define AP_OFFSET 8850000

class ArchipelagoHandler
{
public:
	static void ConnectAP(LoginWindow* login);
	static void DisconnectAP();
	static bool ap_connected;
	static std::string seed;
private:
};
