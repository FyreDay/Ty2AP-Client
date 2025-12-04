#include "pch.h"
#include "InfoWindow.h"

void InfoWindow::ToggleVisibility() {
    isVisible = !isVisible;
}
            void AddLogMessage(const std::string& message);
void InfoWindow::AddLogMessage(const std::string& message) {
    if (logMessages.size() >= maxLogMessages)
        logMessages.pop_front(); // remove oldest
    logMessages.push_back(message); // add newest at the end
}

void InfoWindow::Draw(int outerWidth, int outerHeight, float uiScale, ImFont* font) {
    if (!isVisible)
        return;

    ImGui::Begin("Log");

    if (font) {
        ImGui::PushFont(font);
    }

    for (const auto& msg : logMessages) {
        ImGui::TextUnformatted(msg.c_str());
    }

    if (font) {
        ImGui::PopFont();
    }

    ImGui::End();
}