#pragma once
#include "Window.h"
#include "gui.h"

class LoggerWindow : public Window
{
public:
	LoggerWindow() : Window("Logger") {};
	void ToggleVisibility() override;
	void Draw(int outerWidth, int outerHeight, float uiScale, ImFont* font) override;
	std::string RemoveColorTags(const std::string& text);
	void AddLogMessage(const std::string& message);
	static void Log(const std::string& message);
	static void LogNodes(APClient::PrintJSONArgs data);
private:
	struct LogMessage {
		std::string message;
		std::chrono::steady_clock::time_point timestamp;
	};
	std::vector<LogMessage> visibleMessages;
	std::queue<LogMessage> cachedMessages;
	const int TIMEOUT_SECONDS = 5;
	const size_t MAX_MESSAGES = 10;
	void UpdateVisibleMessages();
	void RenderFormattedText(ImDrawList* draw_list, const char* text, ImVec2 pos);
    const std::unordered_map<std::string, std::string> apKeywordColors = {
        //{"Golden Cog", IM_COL32(252, 210, 16, 255)},
		{"black",   "000000"},
		{"red",     "FF0000"},
		{"green",   "00FF00"},
		{"yellow",  "FFFF00"},
		{"blue",    "0000FF"},
		{"magenta", "FF00FF"},
		{"cyan",    "00FFFF"},
		{"white",   "FFFFFF"},
    };
};