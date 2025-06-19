#pragma once
#include "pch.h"
#include "Window.h"
#include <deque>


class InfoWindow : public Window {
public:
	InfoWindow() : Window("GetObject") {
		isVisible = false;
	};
	void ToggleVisibility() override;
	void Draw(int outerWidth, int outerHeight, float uiScale, ImFont* font) override;
	void AddLogMessage(const std::string& message);
private:
	std::deque<std::string> logMessages;
	const int maxLogMessages = 20;
};