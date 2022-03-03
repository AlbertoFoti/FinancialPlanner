#pragma once

#include "imgui.h"
#include "implot.h"
#include "../Templates/Tab.h"

class FIRE : public Tab {
	Core* core;
    
public:
	FIRE(Core* core);
	void Render();
	void ShowControlPanel(std::string panel_name);
};