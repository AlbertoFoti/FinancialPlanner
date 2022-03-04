#pragma once

#include "imgui.h"
#include "implot.h"
#include "../Templates/Tab.h"

class Investments : public Tab {
	Core* core;
    
public:
	Investments(Core* core);
	void Render();
	void ShowControlPanel(std::string panel_name);
};