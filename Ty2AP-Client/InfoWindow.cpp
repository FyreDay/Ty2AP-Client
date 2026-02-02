#include "pch.h"
#include "InfoWindow.h"

void InfoWindow::ToggleVisibility() {
    isVisible = !isVisible;
}

void InfoWindow::Draw(int outerWidth, int outerHeight, float uiScale) {
    if (!isVisible)
        return;

    // Draw info window
    ImGui::SetNextWindowSizeConstraints(ImVec2(450, 200), ImVec2(450, 500));
    ImGui::Begin(name.c_str(), &isVisible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(50, 50));
    ImGui::PopStyleVar();

    for (const auto& info : AllInfo) {
        CreateTree(info);
    }

    ImGui::End();
}

void InfoWindow::CreateTree(LevelCollectibleInfo levelInfo) {
    if (ImGui::CollapsingHeader(levelInfo.levelName.c_str())) {
        ImGui::PushID(levelInfo.levelName.c_str());

        if (!levelInfo.steves.empty()) {
            ImGui::Indent(20);
            if (ImGui::TreeNode("Cogs")) {
                ImGui::Indent(20);
                for (auto& [collectibleName, description] : levelInfo.cogs) {
                    ImGui::TextWrapped("%s", collectibleName.c_str());
                    ImGui::Indent(20);
                    ImGui::TextWrapped("%s", description.c_str());
                    ImGui::Unindent(20);
                }
                ImGui::Unindent(20);
                ImGui::TreePop();
            }
            ImGui::Unindent(20);
        }

        if (!levelInfo.bilbies.empty()) {
            ImGui::Indent(20);
            if (ImGui::TreeNode("Bilbies")) {
                ImGui::Indent(20);
                for (auto& [collectibleName, description] : levelInfo.bilbies) {
                    ImGui::TextWrapped("%s", collectibleName.c_str());
                    ImGui::Indent(20);
                    ImGui::TextWrapped("%s", description.c_str());
                    ImGui::Unindent(20);
                }
                ImGui::Unindent(20);
                ImGui::TreePop();
            }
            ImGui::Unindent(20);
        }

        if (!levelInfo.orbs.empty()) {
            ImGui::Indent(20);
            if (ImGui::TreeNode("Kromium Orbs")) {
                ImGui::Indent(20);
                for (auto& [collectibleName, description] : levelInfo.orbs) {
                    ImGui::TextWrapped("%s", collectibleName.c_str());
                    ImGui::Indent(20);
                    ImGui::TextWrapped("%s", description.c_str());
                    ImGui::Unindent(20);
                }
                ImGui::Unindent(20);
                ImGui::TreePop();
            }
            ImGui::Unindent(20);
        }

        if (!levelInfo.pictureFrames.empty()) {
            ImGui::Indent(20);
            if (ImGui::TreeNode("Picture Frames")) {
                ImGui::Indent(20);
                for (auto& [collectibleName, description] : levelInfo.pictureFrames) {
                    ImGui::TextWrapped("%s", collectibleName.c_str());
                    ImGui::Indent(20);
                    ImGui::TextWrapped("%s", description.c_str());
                    ImGui::Unindent(20);
                }
                ImGui::Unindent(20);
                ImGui::TreePop();
            }
            ImGui::Unindent(20);
        }

        if (!levelInfo.steves.empty()) {
            ImGui::Indent(20);
            if (ImGui::TreeNode("Steves")) {
                ImGui::Indent(20);
                for (auto& [collectibleName, description] : levelInfo.steves) {
                    ImGui::TextWrapped("%s", collectibleName.c_str());
                    ImGui::Indent(20);
                    ImGui::TextWrapped("%s", description.c_str());
                    ImGui::Unindent(20);
                }
                ImGui::Unindent(20);
                ImGui::TreePop();
            }
            ImGui::Unindent(20);
        }

        if (!levelInfo.frills.empty()) {
            ImGui::Indent(20);
            if (ImGui::TreeNode("Disguised Frills")) {
                ImGui::Indent(20);
                for (auto& [collectibleName, description] : levelInfo.frills) {
                    ImGui::TextWrapped("%s", collectibleName.c_str());
                    ImGui::Indent(20);
                    ImGui::TextWrapped("%s", description.c_str());
                    ImGui::Unindent(20);
                }
                ImGui::Unindent(20);
                ImGui::TreePop();
            }
            ImGui::Unindent(20);
        }

        ImGui::PopID();
    }
}