#include "IncomeExpenses.h"

IncomeExpenses::IncomeExpenses(Core* core) {
	this->core = core;
}

void IncomeExpenses::Render()
{
	ImGui::Text("Income/Expenses Tab");

	ShowControlPanel("Income/Expenses Control Panel");
}

void IncomeExpenses::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());
	ImGui::Text("Control Panel : Income/Expenses");
	ImGui::End();
}
