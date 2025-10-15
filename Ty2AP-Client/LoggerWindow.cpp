#include "pch.h"
#include "LoggerWindow.h"

void LoggerWindow::ToggleVisibility() {
    isVisible = !isVisible;
}

void LoggerWindow::Draw(int outerWidth, int outerHeight, float uiScale, ImFont* font) {
    if (!isVisible)
        return;


    UpdateVisibleMessages();

    // Render the Logger window at the bottom-left
    ImGui::SetNextWindowPos(ImVec2(10, outerHeight - 500 - 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_Always);
    ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);
    
    if (font) {
        ImGui::PushFont(font);
    }

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
    if (font) {
        ImGui::PopFont();
    }
    ImGui::End();
}

// Function to remove color tags from the text
std::string LoggerWindow::RemoveColorTags(const std::string& text) {
    std::string result;
    result.reserve(text.size());

    bool insideTag = false;
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '[') {
            insideTag = true;
        }
        else if (text[i] == ']') {
            insideTag = false;
        }
        else if (!insideTag) {
            result += text[i];
        }
    }

    return result;
}


void LoggerWindow::RenderFormattedText(ImDrawList* draw_list, const char* text, ImVec2 pos) {
    // Define keywords and associated colors
    static const std::unordered_map<std::string, ImU32> keywordColors = {
        //{"Golden Cog", IM_COL32(252, 210, 16, 255)},
    };

    ImU32 current_color = IM_COL32(255, 255, 255, 255); // Default white
    const char* segment_start = text;
    const char* s = text;
    float x_pos = pos.x;

    while (*s) {
        // Check for color tag "[color=XXXXXX]"
        if (*s == '[' && *(s + 1) == 'c' && strncmp(s, "[color=", 7) == 0) {
            if (segment_start < s) {
                draw_list->AddText(ImVec2(x_pos, pos.y), current_color, segment_start, s);
                x_pos += ImGui::CalcTextSize(segment_start, s).x;
            }

            // Parse color code
            s += 7; // Skip "[color="
            unsigned int r, g, b, a = 255;
            if (sscanf_s(s, "%2x%2x%2x%2x", &r, &g, &b, &a) >= 3) {
                current_color = IM_COL32(r, g, b, a);
            }

            // Move past the color tag
            while (*s && *s != ']') s++;
            if (*s == ']') s++;
            segment_start = s;
            continue;
        }

        // Check for keywords and apply coloring
        for (const auto& [keyword, color] : keywordColors) {
            size_t len = keyword.length();
            if (strncmp(s, keyword.c_str(), len) == 0 && (s[len] == ' ' || s[len] == '\0')) {
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
                break; // Restart loop from new position
            }
        }

        s++;
    }

    // Draw remaining text
    if (segment_start < s) {
        draw_list->AddText(ImVec2(x_pos, pos.y), current_color, segment_start, s);
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

void LoggerWindow::LogNodes(APClient::PrintJSONArgs printdata)
{
    for (auto& window : GUI::windows) {
        if (auto logger = dynamic_cast<LoggerWindow*>(window.get())) {
            std::string message;
            for (const auto& text : printdata.data) {
                if (text.type == "") {
                    auto it = logger->apKeywordColors.find(text.color);
                    std::string colorHex = (it != logger->apKeywordColors.end()) ? it->second : "FFFFFF";
                    message += text.text;
                }
                if (text.type == "player_id") {
                    int value = std::atoi(text.text.c_str());
                    message += ArchipelagoHandler::GetPlayerName(value);
                }
                if (text.type == "item_id") {
                    message += ArchipelagoHandler::GetItemName(printdata.item->item, printdata.item->player);
                }
                if (text.type == "location_id") {
                    int value = std::atoi(text.text.c_str());
                    message += ArchipelagoHandler::GetLocationName(value, printdata.item->player);
                }
            }
            logger->AddLogMessage(message);

            break;
        }
    }
    
}
