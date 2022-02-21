#pragma once

#include "imgui.h"
#include "../Templates/Tab.h"
#include <string>

class NetWorth : public Tab {
	Core* core;
public:
	NetWorth(Core* core);
	void Render();
	void ShowControlPanel(std::string panel_name);
};