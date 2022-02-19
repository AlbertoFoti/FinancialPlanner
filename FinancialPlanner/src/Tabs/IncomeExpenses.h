#pragma once

#include "imgui.h"
#include "../Core/Core.h"
#include "../Templates/Tab.h"

class IncomeExpenses : public Tab {
public:
	void Render();
	void ShowControlPanel(std::string panel_name);
};