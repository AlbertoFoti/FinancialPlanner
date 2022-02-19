#pragma once

#include "imgui.h"
#include "Core.h"
#include "Tab.h"

class IncomeExpenses : public Tab {
public:
	void Render();
	void ShowControlPanel(std::string panel_name);
};