#include "pch.h"
#include "TrackerWindow.h"

void TrackerWindow::ToggleVisibility() {
    isVisible = !isVisible;
    if (!isVisible)
        LoggerWindow::Log("When the tracker is toggled off, you can view it while holding T.");
}

void TrackerWindow::Draw(int outerWidth, int outerHeight, float uiScale) {
    if (!isVisible && !visibleOverride)
        return;

    if (!GameHandler::IsInGame())
        return;

    auto windowWidth = 690 * uiScale;
    auto windowHeight = 340 * uiScale;
    auto iconSize = 48 * uiScale;
    auto padding = 10 * uiScale;
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.5));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f * uiScale, 4.5 * uiScale));
    ImGui::Begin(
        name.c_str(),
        nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_AlwaysAutoResize
    );
    ImGui::SetWindowFontScale(uiScale + 0.3f);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    bool b = false;
    float brightness = 0.0f;
    ImVec4 tintColor;
    const float lowBright = 0.4f;
    const float semiBright = 0.7f;
    const float fullBright = 1.0f;

    auto cursorPos = ImGui::GetCursorScreenPos();
    
    int count = ArchipelagoHandler::customSaveData->missionsCompleted;
    int maxCount = ArchipelagoHandler::slotdata->missionsToGoal;
    brightness = lowBright;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::ImageWithBg((ImTextureID)(intptr_t)GUI::icons["mission"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), tintColor);
    std::string missionCount = std::to_string(count) + "/" + std::to_string(maxCount);
    ImGui::PushFont(GUI::fontSmall);
    ImVec2 min = ImGui::GetItemRectMin();
    ImVec2 max = ImGui::GetItemRectMax();
    ImVec2 center = ImVec2((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f);
    ImVec2 textSize = ImGui::CalcTextSize(missionCount.c_str());
    ImVec2 textPos = ImVec2(center.x - textSize.x * 0.5f, center.y - textSize.y * 0.5f);
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), missionCount.c_str());
    ImGui::PopFont();
    ImGui::SameLine();

    ImGui::SameLine();

    brightness = fullBright;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::ImageWithBg((ImTextureID)(intptr_t)GUI::icons["battlekey"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), tintColor);

    ImGui::SameLine();

    b = ArchipelagoHandler::customSaveData->hasThermoKey;
    brightness = b ? fullBright : lowBright;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::ImageWithBg((ImTextureID)(intptr_t)GUI::icons["thermokey"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), tintColor);

    ImGui::SameLine();

    b = ArchipelagoHandler::customSaveData->hasLifterKey;
    brightness = b ? fullBright : lowBright;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::ImageWithBg((ImTextureID)(intptr_t)GUI::icons["lifterkey"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), tintColor);

    ImGui::SameLine();

    b = ArchipelagoHandler::customSaveData->hasSubKey;
    brightness = b ? fullBright : lowBright;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::ImageWithBg((ImTextureID)(intptr_t)GUI::icons["subkey"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), tintColor);

    ImGui::SameLine();

    b = ArchipelagoHandler::customSaveData->hasBilbyMap;
    brightness = b ? fullBright : lowBright;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::ImageWithBg((ImTextureID)(intptr_t)GUI::icons["bilbymap"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), tintColor);

    ImGui::SameLine();

    b = ArchipelagoHandler::customSaveData->hasCogMap;
    brightness = b ? fullBright : lowBright;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::ImageWithBg((ImTextureID)(intptr_t)GUI::icons["cogmap"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), tintColor);

    ImGui::SameLine();

    b = ArchipelagoHandler::customSaveData->hasSteveMap;
    brightness = b ? fullBright : lowBright;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::ImageWithBg((ImTextureID)(intptr_t)GUI::icons["stevemap"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), tintColor);



    b = std::find(ArchipelagoHandler::customSaveData->UnlockedBarriers.begin(), ArchipelagoHandler::customSaveData->UnlockedBarriers.end(), 980) != ArchipelagoHandler::customSaveData->UnlockedBarriers.end();
    brightness = b ? fullBright : lowBright;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::ImageWithBg((ImTextureID)(intptr_t)GUI::icons["barrier_p"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), tintColor);

    ImGui::SameLine();

    b = std::find(ArchipelagoHandler::customSaveData->UnlockedBarriers.begin(), ArchipelagoHandler::customSaveData->UnlockedBarriers.end(), 982) != ArchipelagoHandler::customSaveData->UnlockedBarriers.end();
    brightness = b ? fullBright : lowBright;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::ImageWithBg((ImTextureID)(intptr_t)GUI::icons["barrier_b"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), tintColor);

    ImGui::SameLine();

    b = std::find(ArchipelagoHandler::customSaveData->UnlockedBarriers.begin(), ArchipelagoHandler::customSaveData->UnlockedBarriers.end(), 981) != ArchipelagoHandler::customSaveData->UnlockedBarriers.end();
    brightness = b ? fullBright : lowBright;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    ImGui::ImageWithBg((ImTextureID)(intptr_t)GUI::icons["barrier_f"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), tintColor);
    
    ImGui::SameLine();

    count = ArchipelagoHandler::customSaveData->orbCount;
    brightness = lowBright;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    cursorPos = ImGui::GetCursorScreenPos();
    ImGui::ImageWithBg((ImTextureID)(intptr_t)GUI::icons["orb"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), tintColor);
    min = ImGui::GetItemRectMin();
    max = ImGui::GetItemRectMax();
    center = ImVec2((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f);
    textSize = ImGui::CalcTextSize(std::to_string(count).c_str());
    textPos = ImVec2(
        center.x - textSize.x * 0.5f,
        center.y - textSize.y * 0.5f
    );
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), std::to_string(count).c_str());
    
    ImGui::SameLine();

    count = ArchipelagoHandler::customSaveData->cogCount;
    brightness = lowBright;
    tintColor = ImVec4(brightness, brightness, brightness, 1.0f);
    cursorPos = ImGui::GetCursorScreenPos();
    ImGui::ImageWithBg((ImTextureID)(intptr_t)GUI::icons["cog"], ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0, 0, 0, 0), tintColor);
    min = ImGui::GetItemRectMin();
    max = ImGui::GetItemRectMax();
    center = ImVec2((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f);
    textSize = ImGui::CalcTextSize(std::to_string(count).c_str());
    textPos = ImVec2(
        center.x - textSize.x * 0.5f,
        center.y - textSize.y * 0.5f
    );
    drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), std::to_string(count).c_str());
    ImGui::SameLine();

    ImVec2 windowSize = ImGui::GetWindowSize();
    ImGui::SetWindowPos(
        ImVec2(
            (outerWidth - windowSize.x) * 0.5f,
            padding
        ),
        ImGuiCond_Always
    );

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}