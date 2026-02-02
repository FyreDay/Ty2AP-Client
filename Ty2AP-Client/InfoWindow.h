#pragma once
#include "Window.h"
#include "gui.h"
#include "collectible_info.h"

class InfoWindow : public Window
{
public:
	InfoWindow() : Window("Collectible Info") {
		isVisible = false;
	};
	void ToggleVisibility() override;
	void Draw(int outerWidth, int outerHeight, float uiScale) override;
private:
	void CreateTree(LevelCollectibleInfo levelInfo);
};
