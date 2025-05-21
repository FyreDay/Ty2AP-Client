#pragma once
#include "Window.h"
#include "LoginWindow.h"
#include "InfoWindow.h"
#include "pch.h"
#include <cstdio>

class GUI
{
public:
	static inline bool init;
	static inline bool isShown;
	static std::vector<std::unique_ptr<Window>> windows;
	static void Initialize();
	static void DrawUI();
	static bool ImGuiWantCaptureMouse();
	static bool WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};