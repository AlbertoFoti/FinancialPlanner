#pragma once

#include "imgui.h"
#include "implot.h"
#include "../Templates/Tab.h"
#include "../Utility/Utility.h"
#include "Plotter.h"
#include <string>

class NetWorth : public Tab {
	Core* core;
	std::vector<NW_record_p> NW_records;
public:
	NetWorth(Core* core);
	void Render();
	void ShowControlPanel(std::string panel_name);
};