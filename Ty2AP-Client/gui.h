#pragma once
#include "pch.h"
#include "Window.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "resource.h"
#include "imgui_stdlib.h"
#include "InfoWindow.h"
#include "LoginWindow.h"
#include "LoggerWindow.h"
#include "TrackerWindow.h"
#include <cstdio>
#include "windows.h"

class GUI
{
public:
	static inline bool init;
	static inline bool isShown;
	static inline bool filterToSelf = false;
	static std::vector<std::unique_ptr<Window>> windows;
	static void Initialize();
	static inline ImFont* fontRegular;
	static inline ImFont* fontLarge;
	static inline ImFont* fontSmall;
	static void DrawUI();
	static bool ImGuiWantCaptureMouse();
	static bool WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static std::map<std::string, GLuint> icons;
private:
	static bool LoadIcons();
	static GLuint LoadTextureFromResource(int resourceId);
};