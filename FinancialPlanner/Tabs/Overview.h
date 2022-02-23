#pragma once

#include "imgui.h"
#include "implot.h"
#include "../Templates/Tab.h"
#include "Plotter.h"

class Overview : public Tab {
	Core* core;
	std::vector<NW_record_p> NW_records;
public:
	Overview(Core* core);
	void Render();
	void ShowControlPanel(std::string panel_name);
};