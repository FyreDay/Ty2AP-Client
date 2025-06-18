#include "pch.h"
#include "ArchipelagoHandler.h"
#include "LoginWindow.h"
#include <apuuid.hpp>

#define UUID_FILE "uuid" // TODO: place in %appdata%
#define GAME_NAME "Ty the Tasmanian Tiger 2"

#define VERSION_STRING "0.0.1"

using nlohmann::json;
bool ap_connect_sent = false;
bool is_https = false;
bool is_wss = false;
bool is_ws = false;
std::string ArchipelagoHandler::uuid = "";
bool ArchipelagoHandler::ap_connected = false;
std::string ArchipelagoHandler::seed;
bool ArchipelagoHandler::ap_sync_queued = false;

SlotData* ArchipelagoHandler::slotdata = new SlotData();
APSaveData* ArchipelagoHandler::customSaveData = new APSaveData();
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

	ap->set_slot_connected_handler([login](const json& data) {
		LoggerWindow::Log("Connected");
		login->SetMessage("Connected");
		bool deathlink = false;
		std::list<std::string> tags = {};
		if (deathlink)
			tags.push_back("DeathLink");
		ap->ConnectUpdate(false, 0b111, true, tags);
		ap->StatusUpdate(APClient::ClientStatus::PLAYING);

		if (data.find("ModVersion") != data.end() || data["ModVersion"] != VERSION_STRING)
			LoggerWindow::Log("Your client and apworld versions do not match.");

		if (data.find("ReqBosses") != data.end()) {
			slotdata->requireBosses = data["ReqBosses"].get<int>() == 1;
		}

		if (data.find("MissionsToGoal") != data.end()) {
			slotdata->missionsToGoal = data["MissionsToGoal"].get<int>() == 1;
		}

		if (data.find("BarrierUnlock") != data.end()) {
			slotdata->barrierUnlockStyle = static_cast<BarrierUnlock>(data["BarrierUnlock"].get<int>());
		}

		if (data.find("CogPrices") != data.end() && data["CogPrices"].is_array()) {
			auto cogprices = data["CogPrices"].get<std::vector<int>>();
			slotdata->cogPrices = cogprices;
		}
		if (data.find("OrbPrices") != data.end() && data["OrbPrices"].is_array()) {
			auto orbprices = data["OrbPrices"].get<std::vector<int>>();
			slotdata->orbPrices = orbprices;
		}

		GameHandler::SetMissionRequirements(slotdata->barrierUnlockStyle, slotdata->missionsToGoal);
	});
	ap->set_slot_disconnected_handler([login]() { 
		LoggerWindow::Log("Slot disconnected");
		login->SetMessage("Disconnected");
	});
	ap->set_slot_refused_handler([login](const std::list<std::string>& errors) {
		for (const auto& error : errors) {
			LoggerWindow::Log("Connection refused : " + error);
			login->SetMessage("Connection refused");
		}
	});
	ap->set_room_info_handler([login]() {
		std::list<std::string> tags;
		ap->ConnectSlot(login->slot, login->password, 0b111, tags, { 0,6,0 });
	});

	ap->set_items_received_handler([](const std::list<APClient::NetworkItem>& items) {
		for (const auto& item : items) {
			
			std::string sender = ap->get_player_alias(item.player);
			//std::string location = ap->get_location_name(item.location, );
			ItemHandler::HandleItem(item);
		}
	});
	ap->set_location_info_handler([](const std::list<APClient::NetworkItem>& items) {
		ItemHandler::FillShopItemNames(items);
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
	if (ap) {
		ap->poll();
		if (GameHandler::IsInGame()) {
			if (!GameHandler::hasRunSetup) {
				GameHandler::RunLoadSetup(slotdata->barrierUnlockStyle == BarrierUnlock::OpenWorld);
			}
		}
		else {
			GameHandler::hasRunSetup = false;
		}
	}
}


std::string ArchipelagoHandler::GetItemName(int64_t id, int player) {
	if (ap) {
		return ap->get_item_name(id, ap->get_player_game(player));
	}
	return "";
}

std::string ArchipelagoHandler::GetItemDesc(int player) {
	if (ap) {
		return "Item for "+ ap->get_player_alias(player) + " playing " + ap->get_player_game(player);
	}
	return "";
}

std::string ArchipelagoHandler::GetSaveIdentifier() {
	if (ap) {
		return ap->get_slot() + "_" + ap->get_seed();
	}
	return "";
}


bool ArchipelagoHandler::ScoutLocations(std::list<int64_t> locations, int create_as_hint) {
	if (ap) {
		return ap->LocationScouts(locations, create_as_hint);
	}
	return false;
}