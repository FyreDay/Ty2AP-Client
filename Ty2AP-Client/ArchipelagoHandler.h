#pragma once
#include "nlohmann/json.hpp"
#include "LoginWindow.h"
#include "LoggerWindow.h"
#include "APSaveData.h"
#define UUID_FILE "uuid"
#define CERT_STORE "cacert.pem"
class LoginWindow;
#define GAME_NAME "Ty the Tasmanian Tiger 2"
class ArchipelagoHandler
{
public:
	static void ConnectAP(LoginWindow* login);
	static void DisconnectAP();
	static void gameFinished();
	static void Poll();
	static void SendLocation(int64_t locationId);
	static std::string uuid;
	static bool ap_connected;
	static std::string seed;
private:
	static bool ap_sync_queued;
	static APClient* ap;
};
