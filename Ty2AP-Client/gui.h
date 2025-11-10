#pragma once
#include "pch.h"
#include "Window.h"
#include "LoginWindow.h"
#include "InfoWindow.h"
#include "LoggerWindow.h"
#include <cstdio>
#include "windows.h"


class GUI
{
public:
	static inline bool init;
	static inline bool isShown;
	static ImFont* tyFont;
	static std::vector<std::unique_ptr<Window>> windows;
	static void Initialize();
	static void DrawUI();
	static bool ImGuiWantCaptureMouse();
	static bool WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};