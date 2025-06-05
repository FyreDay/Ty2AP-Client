#include "pch.h"
#include "LoginWindow.h"

void LoginWindow::ToggleVisibility() {
    isVisible = !isVisible;
}

void LoginWindow::Draw(int outerWidth, int outerHeight, float uiScale) {
    if (!isVisible)
        return;

    // Draw login window
    ImGui::Begin(name.c_str(), &isVisible, ImGuiWindowFlags_AlwaysAutoResize);
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
    ////remove later
    //ImGui::InputText("Mission Id", id, IM_ARRAYSIZE(id));

    //if (ImGui::Button("Find Mission")) {
    //    int oID = 0;
    //    try {
    //        // Try converting the char array to an int using std::stoi
    //        oID = std::stoi(id);

    //        std::optional<MissionWrapper> foundmission = std::optional<MissionWrapper>();
    //        
    //        for (int i = 0; i < 7; i++) {
    //            LinkedList<MissionWrapper> list = SaveData::MissionList(i);
    //            std::optional<MissionWrapper> mission = SaveData::findMissionByID(list, oID);
    //            if (mission.has_value()) {
    //                foundmission = mission;
    //                break;
    //            }
    //        }
    //        if (foundmission.has_value()) {
    //            char hexStr[20];

    //            sprintf_s(hexStr, "0x%p", (void*)foundmission.value().address);
    //            mPointer = hexStr;
    //        }
    //        else {
    //            mPointer = "failed";
    //        }

    //        
    //    }
    //    catch (const std::invalid_argument& e) {
    //        SetMessage("ID Not an int");
    //        API::LogPluginMessage(e.what());
    //    }
    //}
    //ImGui::InputText("Pointer", mPointer.data(), mPointer.size(), ImGuiInputTextFlags_ReadOnly);
    //ImGui::InputText("Mission Pointer", mptr, IM_ARRAYSIZE(mptr));
    //ImGui::InputText("Mission State", mstate, IM_ARRAYSIZE(mstate));
    //if (ImGui::Button("Set Mission State")) {
    //    int MPTR = 0;
    //    int MSTATE = 0;
    //    try {
    //        // Try converting the char array to an int using std::stoi
    //        MPTR = static_cast<int>(std::stoul(mptr, nullptr, 0));
    //        MSTATE = std::stoi(mstate);

    //        Missions::UpdateMissionState((MissionStruct*)MPTR, MSTATE, 0);
    //    }
    //    catch (const std::invalid_argument& e) {
    //        API::LogPluginMessage(e.what());
    //    }
    //}
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