#pragma once

#include "imgui.h"
#include "implot.h"
#include "../Templates/Tab.h"

class Forecasting : public Tab {
	Core* core;
    
public:
	Forecasting(Core* core);
	void Render();
	void ShowControlPanel(std::string panel_name);
};