#pragma once

#include "imgui.h"
#include "implot.h"
#include "../Templates/Tab.h"
#include "Plotter.h"
#include <string>
#include <time.h>

class NetWorth : public Tab {
	Core* core;
public:
	NetWorth(Core* core);
	void Render();
	void ShowControlPanel(std::string panel_name);
};