#include "pch.h"
#include "ArchipelagoHandler.h"
#include "LoginWindow.h"
#include <apuuid.hpp>

#define UUID_FILE "uuid" // TODO: place in %appdata%
#define GAME_NAME "Ty the Tasmanian Tiger 2"

#define VERSION_TUPLE {0,5,1}

using nlohmann::json;
bool ap_connect_sent = false;
bool is_https = false;
bool is_wss = false;
bool is_ws = false;
std::string ArchipelagoHandler::uuid = "";
bool ArchipelagoHandler::ap_connected = false;
std::string ArchipelagoHandler::seed;
bool ArchipelagoHandler::ap_sync_queued = false;

APClient* ArchipelagoHandler::ap = nullptr;

void ArchipelagoHandler::DisconnectAP() {
	LoggerWindow::Log("Try Disconnecting");
}

void ArchipelagoHandler::ConnectAP(LoginWindow* login)
{
	std::string uri = login->server;
	std::string uuid = ap_get_uuid(UUID_FILE,
		uri.empty() ? APClient::DEFAULT_URI :
		is_ws ? uri.substr(5) :
		is_wss ? uri.substr(6) :
		uri);

	if (ap != nullptr) {
		ap->reset();
	}

	ap = new APClient(uuid, GAME_NAME, login->server);
	ap_sync_queued = false;

	ap->set_slot_connected_handler([](const json& data) {
		LoggerWindow::Log("connected");
	});
	ap->set_slot_disconnected_handler([]() {
		LoggerWindow::Log("Slot disconnected");
	});
	ap->set_slot_refused_handler([](const std::list<std::string>& errors) {
		for (const auto& error : errors) {
			LoggerWindow::Log("Connection refused : " + error);
		}
	});
	ap->set_room_info_handler([login]() {
		std::list<std::string> tags;
		//if (GameHook->dIsDeathLink) { tags.push_back("DeathLink"); }
		ap->ConnectSlot(login->slot, login->password, 5, tags, { 0,6,0 });
	});

	ap->set_items_received_handler([](const std::list<APClient::NetworkItem>& items) {
		for (const auto& item : items) {
			std::string itemname = ap->get_item_name(item.item, GAME_NAME);
			std::string sender = ap->get_player_alias(item.player);
			//std::string location = ap->get_location_name(item.location, );

			//Check if we should ignore this item
			if (item.index < APSaveData::pLastReceivedIndex) {
				continue;
			}
			LoggerWindow::Log("item recieved: " + itemname);
			APSaveData::pLastReceivedIndex++;
		}
	});
}

void ArchipelagoHandler::SendLocation(int64_t locationId) {
	std::list<int64_t> check;
	check.push_back(locationId);
	ap->LocationChecks(check);
}

void ArchipelagoHandler::gameFinished() {
	ap->StatusUpdate(APClient::ClientStatus::GOAL);
}

void ArchipelagoHandler::Poll() {
	if (ap) ap->poll();
}