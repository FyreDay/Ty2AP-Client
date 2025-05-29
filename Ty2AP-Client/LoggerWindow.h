#pragma once
#include "Window.h"
#include "gui.h"

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