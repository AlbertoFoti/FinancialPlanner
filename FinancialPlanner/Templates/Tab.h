#pragma once

#include "imgui.h"
#include <string>

class Tab {
public:
	virtual void Render();
	virtual void ShowControlPanel(std::string panel_name);
};