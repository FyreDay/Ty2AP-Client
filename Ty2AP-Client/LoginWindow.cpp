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

static std::vector<MKTObject*> FindObjectsByType(MKTObject* startObject, const char* targetTypeName) {
    std::vector<MKTObject*> result;

    // 1. Walk backwards to the head
    MKTObject* current = startObject;
    while (current->prev != nullptr) {
        current = current->prev;
    }

    // 2. Walk forwards and search
    while (current != nullptr) {
        TypeInfo* typeInfo = reinterpret_cast<TypeInfo*>(current->typeInfoPtr);
        const char* typeName = typeInfo->typeNamePtr;

        if (strcmp(typeName, targetTypeName) == 0) {
            uintptr_t addr = (uintptr_t)current;
            std::stringstream ss;
            ss << std::hex << std::showbase << addr;
            API::LogPluginMessage(ss.str());
            result.push_back(current);
        }

        current = current->next;
    }

    return result;
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

    if (ImGui::Button("Search For mission lights")) {
        FindObjectsByType((MKTObject*)(MKObject::GetMKObject(100)), "P1066_MissionLight");
    }
    if (ImGui::Button("Try kill Ty")) {
        typedef void(__thiscall* StateTransitionFunc)(void* thisPtr, int firstArg, int secondArg);
        StateTransitionFunc transitionFunc = (StateTransitionFunc)(Core::moduleBase + 0x0022c7d0);
        void* tyStateHandler = (void*)(MKObject::GetMKObject(204) + 0x470);
        int stateid = 0xe;
        int source = 0x0;
        transitionFunc(tyStateHandler, stateid, source);
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