#include "pch.h"
#include "LoggerWindow.h"

void LoggerWindow::ToggleVisibility() {
    isVisible = !isVisible;
}

void LoggerWindow::Draw(int outerWidth, int outerHeight, float uiScale) {
    if (!isVisible)
        return;

    UpdateVisibleMessages();

    // Render the Logger window at the bottom-left
    ImGui::SetNextWindowPos(ImVec2(10, outerHeight - 500 - 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Always);
    ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);
    ImGui::SetWindowFontScale(uiScale + 0.3f);

    // Get the window draw list for custom drawing
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 window_pos = ImGui::GetCursorScreenPos();
    ImVec2 window_size = ImGui::GetContentRegionAvail();

    float y_pos = window_pos.y + window_size.y;

    // Iterate messages in reverse order (newest at bottom)
    for (int i = visibleMessages.size() - 1; i >= 0; --i) {
        const LogMessage& log = visibleMessages[i];
        float x_pos = window_pos.x;
        float max_width = window_size.x;

        // Remove color tags and format the raw text for wrapping
        if (log.message.empty() or &log.message == nullptr)
            continue;
        std::string rawMessage = RemoveColorTags(log.message);

        std::vector<std::string> wrappedLines;
        std::string currentLine;
        float totalTextHeight = 0.0f;
        float maxLineWidth = 0.0f; // Track the longest line's width

        std::istringstream words(rawMessage);
        std::string word;

        // Wrap the plain raw text before applying colors
        while (words >> word) {
            std::string testLine = currentLine.empty() ? word : currentLine + " " + word;
            ImVec2 textSize = ImGui::CalcTextSize(testLine.c_str());

            // If text exceeds max width, wrap to the next line
            if (textSize.x > max_width && !currentLine.empty()) {
                wrappedLines.push_back(currentLine);
                maxLineWidth = std::max(maxLineWidth, ImGui::CalcTextSize(currentLine.c_str()).x);
                totalTextHeight += ImGui::CalcTextSize(currentLine.c_str()).y;
                currentLine = word;  // Start new line with current word
            }
            else {
                currentLine = testLine;  // Keep adding words to the line
            }
        }

        // Add the last line if it exists
        if (!currentLine.empty()) {
            wrappedLines.push_back(currentLine);
            maxLineWidth = std::max(maxLineWidth, ImGui::CalcTextSize(currentLine.c_str()).x);
            totalTextHeight += ImGui::CalcTextSize(currentLine.c_str()).y;
        }

        maxLineWidth += 5.0f;

        // Draw the background box for the whole wrapped message
        float messageStartY = y_pos - totalTextHeight - (2.0f * wrappedLines.size());

        // Only add a background box if there's actual text
        if (!wrappedLines.empty()) {
            ImVec2 box_min = ImVec2(window_pos.x, messageStartY);
            ImVec2 box_max = ImVec2(window_pos.x + maxLineWidth, y_pos);
            draw_list->AddRectFilled(box_min, box_max, IM_COL32(0, 0, 0, 100));  // Background for the message
        }

        // Iterate over wrapped lines and render them
        for (auto lineIt = wrappedLines.rbegin(); lineIt != wrappedLines.rend(); ++lineIt) {
            ImVec2 text_size = ImGui::CalcTextSize(lineIt->c_str());
            y_pos -= text_size.y + 2.0f; // Move y-position down for next line

            // Render the formatted text with the current line
            RenderFormattedText(draw_list, lineIt->c_str(), ImVec2(x_pos, y_pos)); // Render the formatted text
        }

        // Add space between messages for separation
        y_pos -= 5.0f;  // Adjust this space for gap between messages
    }
    ImGui::End();
}

// Function to remove color tags from the text
std::string LoggerWindow::RemoveColorTags(const std::string& text) {
    static const std::regex colorTag(R"(\[color\s*=\s*[0-9a-fA-F]{8}\])");
    return std::regex_replace(text, colorTag, "");
}

void LoggerWindow::RenderFormattedText(ImDrawList* draw_list, const char* text, ImVec2 pos) {
    // Define keywords and associated colors
    const std::unordered_map<std::string, ImU32> keywordColors = {
        {"Boomerang", IM_COL32(0xB8, 0x80, 0x5E, 0xFF)},
        {"Multirang", IM_COL32(0xC4, 0x86, 0x3B, 0xFF)},
        {"Flamerang", IM_COL32(0xD1, 0x50, 0x36, 0xFF)},
        {"Lavarang", IM_COL32(0xC9, 0x25, 0x04, 0xFF)},
        {"Frostyrang", IM_COL32(0x4B, 0xAA, 0xF2, 0xFF)},
        {"Freezerang", IM_COL32(0x13, 0x92, 0xF2, 0xFF)},
        {"Zappyrang", IM_COL32(0x4F, 0xE0, 0xE3, 0xFF)},
        {"Thunderang", IM_COL32(0x0F, 0xF2, 0xCC, 0xFF)},
        {"Lasharang", IM_COL32(0xF7, 0x96, 0x05, 0xFF)},
        {"Warperang", IM_COL32(0x96, 0x05, 0xF7, 0xFF)},
        {"Infrarang", IM_COL32(0xF7, 0x05, 0x1D, 0xFF)},
        {"X-Rang", IM_COL32(0xB5, 0x04, 0x15, 0xFF)},
        {"Smasharang", IM_COL32(0x85, 0x83, 0x83, 0xFF)},
        {"Kaboomarang", IM_COL32(0xB3, 0xAF, 0xAF, 0xFF)},
        {"Megarang", IM_COL32(0xC9, 0xC1, 0xC2, 0xFF)},
        {"Omegarang", IM_COL32(0xCF, 0xC0, 0xC2, 0xFF)},
        {"Deadlyrang", IM_COL32(0x9E, 0x3A, 0x44, 0xFF)},
        {"Doomerang", IM_COL32(0xB3, 0xA4, 0x02, 0xFF)},
        {"Aquarang", IM_COL32(0xFF, 0xD0, 0x00, 0xFF)},
        {"Camerang", IM_COL32(0xF5, 0xF2, 0x58, 0xFF)},
        {"Parking Bay", IM_COL32(0x69, 0x69, 0x69, 0xFF)},
        {"Kromium Orb", IM_COL32(0xFC, 0x2B, 0x2B, 0xFF)},
        {"Bilby", IM_COL32(0x51, 0x66, 0xA6, 0xFF)},
        {"Steve", IM_COL32(0x35, 0x91, 0x23, 0xFF)},
        {"Opals", IM_COL32(0xCC, 0x33, 0x4D, 0xFF)},
        {"Platinum Cog", IM_COL32(0xC4, 0xE9, 0xF5, 0xFF)},
        {"Full Pie", IM_COL32(0x9C, 0x8C, 0x52, 0xFF)},
        {"Barriers", IM_COL32(0x77, 0x74, 0x96, 0xFF)},
        {"Sub Bunyip", IM_COL32(0x8A, 0x91, 0x3F, 0xFF)},
        {"Thermo Bunyip", IM_COL32(0x33, 0x2C, 0xF5, 0xFF)},
        {"Lifter Bunyip", IM_COL32(0xE9, 0xED, 0x07, 0xFF)},
        {"Health Paw", IM_COL32(0xD4, 0xA8, 0x17, 0xFF)},
        {"Missing Persons Map", IM_COL32(0x51, 0x66, 0xA6, 0xFF)},
        {"Cog Map", IM_COL32(0xC4, 0xE9, 0xF5, 0xFF)},
        {"Mysterious Anomalies Map", IM_COL32(0x35, 0x91, 0x23, 0xFF)},
        {"Sly", IM_COL32(0xD6, 0x77, 0x2D, 0xFF)},
        {"Fourbie", IM_COL32(0xFF, 0xA9, 0x00, 0xFF)},
        {"Rang", IM_COL32(0xCF, 0x87, 0x00, 0xFF)},
        {"Steve", IM_COL32(0x77, 0xB5, 0x2B, 0xFF)},
        {"Picture Frame", IM_COL32(0xF5, 0xC6, 0x1D, 0xFF)},
        {"Mission", IM_COL32(0x1D, 0xF5, 0x21, 0xFF)},
        {"Shop", IM_COL32(0xF5, 0xE0, 0x1D, 0xFF)},
    };

    ImU32 current_color = IM_COL32(255, 255, 255, 255); // Default white
    const char* segment_start = text;
    const char* s = text;
    float x_pos = pos.x;
    std::cmatch match;

    static const std::regex colorTag(R"(\[color\s*=\s*([0-9a-fA-F]{8})\])");
    while (*s) {
        if (std::regex_search(s, match, colorTag)) {
            if (segment_start < s) {
                draw_list->AddText(ImVec2(x_pos, pos.y), current_color, segment_start, s);
                x_pos += ImGui::CalcTextSize(segment_start, s).x;
            }

            unsigned int rgba;
            sscanf_s(match[1].first, "%8x", &rgba);
            unsigned int r = (rgba >> 24) & 0xFF;
            unsigned int g = (rgba >> 16) & 0xFF;
            unsigned int b = (rgba >> 8) & 0xFF;
            unsigned int a = (rgba >> 0) & 0xFF;
            ImU32 color = IM_COL32(r, g, b, a);
            s = match[0].second;

            segment_start = s;
            continue;
        }

        bool keyword_hit = false;
        for (const auto& [keyword, color] : keywordColors) {
            size_t len = keyword.length();
            if (compareStrings(s, keyword.c_str(), len) && (s[len] == ' ' || s[len] == '\0')) {
                // Draw previous segment before keyword
                if (segment_start < s) {
                    draw_list->AddText(ImVec2(x_pos, pos.y), current_color, segment_start, s);
                    x_pos += ImGui::CalcTextSize(segment_start, s).x;
                }
                // Draw keyword in highlighted color
                draw_list->AddText(ImVec2(x_pos, pos.y), color, s, s + len);
                x_pos += ImGui::CalcTextSize(s, s + len).x;
                s += len;
                segment_start = s;
                keyword_hit = true;
                break; // Restart loop from new position
            }
        }
        if (keyword_hit)
            continue;

        s++;
    }
    if (segment_start < s) {
        draw_list->AddText(ImVec2(x_pos, pos.y), current_color, segment_start, s);
        x_pos += ImGui::CalcTextSize(segment_start, s).x;
    }
}

void LoggerWindow::AddLogMessage(const std::string& message) {
    LogMessage logMessage = { message, std::chrono::steady_clock::now() };
    cachedMessages.push(logMessage);
}

void LoggerWindow::UpdateVisibleMessages() {
    auto now = std::chrono::steady_clock::now();

    visibleMessages.erase(std::remove_if(visibleMessages.begin(), visibleMessages.end(),
        [now, this](const LogMessage& msg) {
            return std::chrono::duration_cast<std::chrono::seconds>(now - msg.timestamp).count() > TIMEOUT_SECONDS;
        }), visibleMessages.end());

    while (!cachedMessages.empty() && visibleMessages.size() < MAX_MESSAGES) {
        cachedMessages.front().timestamp = now;
        visibleMessages.push_back(cachedMessages.front());
        cachedMessages.pop();
    }
}

void LoggerWindow::Log(const std::string& message)
{
    for (auto& window : GUI::windows) {
        if (auto logger = dynamic_cast<LoggerWindow*>(window.get())) {
            logger->AddLogMessage(message);
            break;
        }
    }
}

