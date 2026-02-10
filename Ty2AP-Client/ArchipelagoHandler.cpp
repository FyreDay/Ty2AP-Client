#include "pch.h"
#include "ArchipelagoHandler.h"
#include "LoginWindow.h"
#include <apuuid.hpp>

#define UUID_FILE "uuid" // TODO: place in %appdata%

using nlohmann::json;
bool ap_connect_sent = false;
bool is_https = false;
bool is_wss = false;
bool is_ws = false;
std::string ArchipelagoHandler::uuid = "";
bool ArchipelagoHandler::ap_connected = false;
std::string ArchipelagoHandler::seed;
std::string ArchipelagoHandler::slotname = "";
bool ArchipelagoHandler::ap_sync_queued = false;
std::atomic<bool> ArchipelagoHandler::polling(false);
bool ArchipelagoHandler::wrongVersion = false;

SlotData* ArchipelagoHandler::slotdata = new SlotData();
APSaveData* ArchipelagoHandler::customSaveData = new APSaveData();
APClient* ArchipelagoHandler::ap = nullptr;

void ArchipelagoHandler::DisconnectAP() {
	polling.store(false);
	GameHandler::hasRunSetup = false;
	LoggerWindow::Log("Try Disconnecting");
}

void ArchipelagoHandler::ConnectAP(LoginWindow* login)
{
	polling.store(true);
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
		
		slotname = login->slot;
		LoggerWindow::Log("Connected");
		login->SetMessage("Connected");
		std::list<std::string> tags = {};
		if (data.find("DeathLink") != data.end()){
			slotdata->deathlink = data["DeathLink"].get<int>() == 1;
			if (slotdata->deathlink) {
				tags.push_back("DeathLink");
			}
		}
		ap->ConnectUpdate(false, 0b111, true, tags);
		ap->StatusUpdate(APClient::ClientStatus::PLAYING);
		if (data.find("ModVersion") == data.end()) {
			login->SetMessage("AP world is not Valid");
			ArchipelagoHandler::DisconnectAP();
			wrongVersion = true;
		}
		else if (data["ModVersion"].get<std::string>() != COMPAT_VERSION) {
			LoggerWindow::Log("Your client and apworld versions do not match.");
			login->SetMessage("AP world "+ data["ModVersion"].get<std::string>() +" incompatible");
			ArchipelagoHandler::DisconnectAP();
			wrongVersion = true;
			return;
		}
		

		if (data.find("ReqBosses") != data.end()) {
			slotdata->requireBosses = data["ReqBosses"].get<int>() == 1;
		}

		if (data.find("MissionsToGoal") != data.end()) {
			slotdata->missionsToGoal = data["MissionsToGoal"].get<int>();
		}
		if (data.find("ProgressiveRangs") != data.end()) {
			slotdata->progressiveRangs = data["ProgressiveRangs"].get<int>() == 1;
		}

		if (data.find("BarrierUnlock") != data.end()) {
			slotdata->barrierUnlockStyle = static_cast<BarrierUnlock>(data["BarrierUnlock"].get<int>());
		}
		if (data.find("SkipCurrawong") != data.end()) {
			slotdata->skipCurrawong = data["SkipCurrawong"].get<int>() == 1;
		}

		if (data.find("RangPrices") != data.end() && data["RangPrices"].is_array()) {
			auto rangPrices = data["RangPrices"].get<std::vector<int>>();
			slotdata->rangPrices = rangPrices;
		}

		if (data.find("SlyPrices") != data.end() && data["SlyPrices"].is_array()) {
			auto slyPrices = data["SlyPrices"].get<std::vector<int>>();
			slotdata->slyPrices = slyPrices;
		}

		if (data.find("TraderBobPrices") != data.end() && data["TraderBobPrices"].is_array()) {
			auto traderBobPrices = data["TraderBobPrices"].get<std::vector<int>>();
			slotdata->traderBobPrices = traderBobPrices;
		}

		if (data.find("CogPrices") != data.end() && data["CogPrices"].is_array()) {
			auto cogPrices = data["CogPrices"].get<std::vector<int>>();
			slotdata->cogPrices = cogPrices;
		}

		if (data.find("OrbPrices") != data.end() && data["OrbPrices"].is_array()) {
			auto orbPrices = data["OrbPrices"].get<std::vector<int>>();
			slotdata->orbPrices = orbPrices;
		}

		if (slotdata->skipCurrawong) {
			GameHandler::removeCurrawong();
		}

		//GameHandler::SetMissionRequirements(slotdata->barrierUnlockStyle, slotdata->missionsToGoal);
		GameHandler::EnableLoadButtons();
	});
	ap->set_slot_disconnected_handler([login]() { 
		LoggerWindow::Log("Slot disconnected");
		if (!wrongVersion) {
			login->SetMessage("Disconnected");
		}
	});
	ap->set_slot_refused_handler([login](const std::list<std::string>& errors) {
		for (const auto& error : errors) {
			LoggerWindow::Log("Connection refused : " + error);
			login->SetMessage("Connection refused");
		}
	});
	ap->set_print_json_handler([](const std::list<APClient::TextNode>& msg) {
		if (GUI::filterToSelf)
		{
			bool hasSelf = false;
			for (const auto& node : msg)
				if (node.player == ap->get_player_number())
					hasSelf = true;
			if (!hasSelf)
				return;
		}
		LoggerWindow::Log(ap->render_json(msg, APClient::RenderFormat::TEXT));
	});
	ap->set_print_handler([](const std::string& msg) {
		LoggerWindow::Log(msg);
		});
	ap->set_room_info_handler([login]() {
		std::list<std::string> tags;
		ap->ConnectSlot(login->slot, login->password, 0b111, tags, { 0,6,0 });
	});

	ap->set_items_received_handler([](const std::list<APClient::NetworkItem>& items) {
		if (!ap->is_data_package_valid()) {
			if (!ap_sync_queued) {
				ap_sync_queued = true;
				ap->Sync();
			}
			return;
		}
		for (const auto& item : items) {
			
			std::string sender = ap->get_player_alias(item.player);
			ItemHandler::HandleItem(item);
		}
	});
	ap->set_location_info_handler([](const std::list<APClient::NetworkItem>& items) {
		ItemHandler::FillShopItemNames(items);
	});
	ap->set_bounced_handler([](const json& cmd) {
		auto tagsIt = cmd.find("tags");
		auto dataIt = cmd.find("data");
		if (slotdata->deathlink) {
			if (tagsIt != cmd.end() && tagsIt->is_array()
				&& std::find(tagsIt->begin(), tagsIt->end(), "DeathLink") != tagsIt->end())
			{
				if (dataIt != cmd.end() && dataIt->is_object()) {
					json data = *dataIt;
					if (data["source"].get<std::string>() != slotname) {
						std::string source = data["source"].is_string() ? data["source"].get<std::string>().c_str() : "???";
						std::string cause = data["cause"].is_string() ? data["cause"].get<std::string>().c_str() : "???";
						LoggerWindow::Log("[color = FFFFFFFF]" + cause);
						GameHandler::KillTy();
					}
				}
				else {
					LoggerWindow::Log("Bad Deathlink");
				}
			}
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
	while (true) {
		if (polling.load() && ap) {
			ap->poll();
			if (GameHandler::IsInGame()) {
				if (!GameHandler::hasRunSetup) {
					GameHandler::RunLoadSetup(slotdata);
				}
			}
			else {
				GameHandler::hasRunSetup = false;
			}
		}
	}
}

const std::vector<std::string> deathCauses{
	"didn\'t catch the boomerang",
	"bit off more than they could chew",
	"turned into a tasmanian fossil",
	"forgot to wear gloves",
	"had trouble in the outhouse",
	"couldn\'t hold their breath",
	"listened to Lenny",
	"got crushed by an icicle",
	"forgot how to glide",
	"bit the ground too hard",
	"fell over",
	"got a little too Australian",
	"didn\'t wear sunblock",
	"didn\'t listen to Lenny",
	"ate too much pie",
	"hugged a crocodile",
	"got lost in the outback",
	"died",
	"forgot eels were electric",
	"got too confident with the didgeridoo",
	"tried to ride a stingray",
	"told a dingo it wasn\'t that scary",
	"got dropkicked by a kangaroo",
	"said it wasn't their fault",
	"blames Boonie",
	"miscounted the multirangs",
	"licked a toad for science",
	"ran out of \'she\'ll be right\' energy mid-jump",
	"tried to code an AP game"
};

std::string GetRandomCause() {
	if (deathCauses.empty()) return ""; // Handle empty list case
	std::random_device rd;  // Obtain a random seed
	std::mt19937 gen(rd()); // Mersenne Twister PRNG
	std::uniform_int_distribution<int> dist(0, deathCauses.size() - 1);
	return deathCauses[dist(gen)];
}

void ArchipelagoHandler::SendDeath() {
	std::string cause = GetRandomCause();
	LoggerWindow::Log("Death Sent:  " + slotname + " " + cause);
	json data{
		{"time", ap->get_server_time()},
		{"cause", slotname + " " + GetRandomCause()},
		{"source", slotname},
	};
	ap->Bounce(data, {}, {}, { "DeathLink" });
}

std::string ArchipelagoHandler::GetItemName(int64_t id, int player) {
	if (ap) {
		return ap->get_item_name(id, ap->get_player_game(player));
	}
	return "item id: " + id;
}

std::string ArchipelagoHandler::GetLocationName(int64_t id, int player) {
	if (ap) {
		return ap->get_location_name(id, ap->get_player_game(player));
	}
	return "loc id: " + id;
}

int ArchipelagoHandler::CountLocationsChecked(const std::list<int64_t>& locationIds)
{
	const auto& checked = ap->get_checked_locations();
	std::size_t count = 0;
	for (int64_t id : locationIds) {
		if (std::find(checked.begin(), checked.end(), id) != checked.end()) {
			++count;
		}
	}
	return count;
}

bool ArchipelagoHandler::IsLocationChecked(int64_t locationId)
{
	const auto& checked = ap->get_checked_locations();
	return std::find(checked.begin(), checked.end(), locationId) != checked.end();
}

std::string ArchipelagoHandler::GetPlayerName(int player){
	if (ap) {
		return ap->get_player_alias(player);
	}
	return "player slot: " + player;
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