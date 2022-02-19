#pragma once

#include "imgui.h"
#include "Tab.h"
#include <string>

class NetWorth : public Tab {
public:
	void Render();
	void ShowControlPanel(std::string panel_name);
};