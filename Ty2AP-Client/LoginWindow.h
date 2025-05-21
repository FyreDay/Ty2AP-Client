#pragma once
#include "ArchipelagoHandler.h"
#include "Window.h"

class LoginWindow : public Window {
public:
    LoginWindow() : Window("Login") {
        std::string savedServer, savedSlot, savedPassword;
        isVisible = false;
        if (LoadLoginData(savedServer, savedSlot, savedPassword)) {
            // If data was loaded successfully, pre-fill the fields
            strncpy_s(server, savedServer.c_str(), sizeof(server));
            strncpy_s(slot, savedSlot.c_str(), sizeof(slot));
            strncpy_s(password, savedPassword.c_str(), sizeof(password));
        }
    };
    void ToggleVisibility() override;
    void Draw(int outerWidth, int outerHeight, float uiScale) override;
    void SetMessage(std::string);
    bool LoadLoginData(std::string& server, std::string& slot, std::string& password);
    void SaveLoginData(const std::string& server, const std::string& slot, const std::string& password);
    char server[128] = "archipelago.gg:";
    char password[128] = "";
    char slot[128] = "";
private:
    std::string message;
};

