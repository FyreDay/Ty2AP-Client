#pragma once
#include "Window.h"
#include "gui.h"
#include <cctype> 
#include "ArchipelagoHandler.h"
#include <algorithm> 
#include <regex>

class LoggerWindow : public Window
{
public:
	LoggerWindow() : Window("Logger") {};
	void ToggleVisibility() override;
	void Draw(int outerWidth, int outerHeight, float uiScale) override;
	std::string RemoveColorTags(const std::string& text);
	void AddLogMessage(const std::string& message);
	static void Log(const std::string& message);
private:
	bool compareStrings(const char* a, const char* b, size_t len)
	{
		for (size_t i = 0; i < len; ++i) {
			if (a[i] == '\0' || b[i] == '\0')
				return false;

			if (std::tolower(static_cast<unsigned char>(a[i])) !=
				std::tolower(static_cast<unsigned char>(b[i])))
				return false;
		}
		return true;
	}

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
};