#pragma once

#include "imgui.h"
#include "../Core/Core.h"
#include "../Templates/Tab.h"

class IncomeExpenses : public Tab {
	Core* core;
	MonthlyTransactions_p monthlyReport;
public:
	IncomeExpenses(Core* core);
	void Render();
	void ShowControlPanel(std::string panel_name);
};