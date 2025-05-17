#include "pch.h"
#include "InfoWindow.h"

void InfoWindow::ToggleVisibility() {
    isVisible = !isVisible;
}

void InfoWindow::AddLogMessage(const std::string& message) {
    if (logMessages.size() >= maxLogMessages)
        logMessages.pop_front(); // remove oldest
    logMessages.push_back(message); // add newest at the end
}

void InfoWindow::Draw(int outerWidth, int outerHeight, float uiScale) {
    if (!isVisible)
        return;

    ImGui::Begin("Log");

    for (const auto& msg : logMessages) {
        ImGui::TextUnformatted(msg.c_str());
    }

    ImGui::End();
}