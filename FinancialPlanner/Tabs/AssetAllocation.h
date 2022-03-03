#pragma once

#include "imgui.h"
#include "implot.h"
#include "../Templates/Tab.h"

class AssetAllocation : public Tab {
	Core* core;
    
public:
	AssetAllocation(Core* core);
	void Render();
	void ShowControlPanel(std::string panel_name);
};