#include "pch.h"
#include "ArchipelagoHandler.h"
#include "LoginWindow.h"

using nlohmann::json;

bool ArchipelagoHandler::ap_connected = false;
std::string ArchipelagoHandler::seed;

void ArchipelagoHandler::DisconnectAP() {
    //LoggerWindow::Log("Disconnected");
    //SetAPStatus("Disconnected", 1);
    //polling = false;
    //MulTyHandler::IsRunning = false;
    //ap_connect_sent = false;
    //ap_connected = false;
    //if (GameState::onLoadScreenOrMainMenu())
    //    GameHandler::SetLoadActive(false);
    //else
    //    GameState::forceMainMenu();
}

void ArchipelagoHandler::ConnectAP(LoginWindow* login)
{
}