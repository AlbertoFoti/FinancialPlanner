#include "IncomeExpenses.h"

IncomeExpenses::IncomeExpenses(Core* core) {
	this->core = core;
}

void IncomeExpenses::Render()
{
	ImGui::Text("Income/Expenses Tab");

	this->monthlyReport = this->core->getMonthlyTransactionsReport();

	static int year = 2021;
	ImGui::SliderInt("##slider_year", &year, 2000, 2100);
	enum Element { elem_January, elem_February, elem_March, elem_April, elem_May, elem_June, elem_July, elem_August, elem_September, elem_October, elem_November, elem_December, elem_Count};
	static int month = elem_January;
	const char* elems_names[elem_Count] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
	const char* elem_name = (month >= 0 && month < elem_Count) ? elems_names[month] : "Unknown";
	ImGui::SliderInt("##slider_months", &month, 0, elem_Count - 1, elem_name);
	ImGui::SameLine();

	if(ImGui::Button("Get transactions"))
	{
		// Get Transactions
		this->monthlyReport = this->core->getMonthlyTransactionsReportFromDb(month+1, year);
	}

	ImGui::Text("%d / %d", monthlyReport->Month, monthlyReport->Year);
	ImGui::Spacing();
	for (int i = 0; i < monthlyReport->transactions.size(); i++) {
		ImGui::Separator();
		ImGui::Text("%s, %.2f", monthlyReport->transactions[i]->Category.c_str(), monthlyReport->transactions[i]->Amount);
	}

	ShowControlPanel("Income/Expenses Control Panel");
}

void IncomeExpenses::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());
	ImGui::Text("Control Panel : Income/Expenses");
	ImGui::End();
}
