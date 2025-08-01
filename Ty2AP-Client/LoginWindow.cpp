#include "pch.h"
#include "LoginWindow.h"

void LoginWindow::ToggleVisibility() {
    isVisible = !isVisible;
}

static struct MKTObject {
    uintptr_t padding0[2];   // 0x00 to 0x07
    uintptr_t typeInfoPtr;   // 0x08
    uintptr_t padding1[11];  // 0x0C to 0x37
    MKTObject* prev;            // 0x38
    MKTObject* next;            // 0x3C
};

static struct TypeInfo {
    uintptr_t padding0[2];   // 0x00 to 0x07
    const char* typeNamePtr; // 0x08
    uintptr_t padding1[11];  // 0x0C to 0x3B
    MKTObject* owningObject;    // 0x3C
};

void LoginWindow::Draw(int outerWidth, int outerHeight, float uiScale, ImFont* font) {
    if (!isVisible)
        return;

    // Draw login window
    ImGui::Begin(name.c_str(), &isVisible, ImGuiWindowFlags_AlwaysAutoResize);

    if (font) {
        ImGui::PushFont(font);
    }

    ImGui::InputText("Server", server, IM_ARRAYSIZE(server));
    ImGui::InputText("Password", password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);
    ImGui::InputText("Slot Name", slot, IM_ARRAYSIZE(slot));

    if (!ArchipelagoHandler::ap_connected) {
        if (ImGui::Button("Connect")) {
            if (strlen(server) > 0 && strlen(slot) > 0) {
                SaveLoginData(server, slot, password);
                ArchipelagoHandler::ConnectAP(this);
                SetMessage("Connecting to " + std::string(server) + "...");
            }
            else {
                SetMessage("Please enter server and slot name");
            }
        }
    }
    else {
        if (ImGui::Button("Disconnect")) {
            ArchipelagoHandler::DisconnectAP();
            SetMessage("");
        }
    }


    ImGui::TextWrapped("%s", message.c_str());
    //if (ImGui::Button("Kill Ty")) {
    //    GameHandler::KillTy();
    //}
    if (font) {
        ImGui::PopFont();
    }
    ImGui::End();
}






void LoginWindow::SetMessage(std::string newMessage) {
    message = newMessage;
}

void LoginWindow::SaveLoginData(const std::string& server, const std::string& slot, const std::string& password) {
    nlohmann::json jsonData;
    jsonData["server"] = server;
    jsonData["slot"] = slot;
    jsonData["password"] = password;

    // Save the JSON data to a file
    std::ofstream file("./connection.json");
    file << jsonData.dump(4);  // Pretty print with an indentation of 4
    file.close();
}

bool LoginWindow::LoadLoginData(std::string& server, std::string& slot, std::string& password) {
    std::ifstream file("./connection.json");

    if (!file.is_open()) {
        return false;  // Return false if the file doesn't exist
    }

    nlohmann::json jsonData;
    file >> jsonData;
    file.close();

    // Retrieve data from the JSON object
    server = jsonData.value("server", "");
    slot = jsonData.value("slot", "");
    password = jsonData.value("password", "");

    return true;  // Return true if loading was successful
}