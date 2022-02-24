#pragma once

#include "imgui.h"
#include "../Core/Core.h"
#include "../Templates/Tab.h"

class IncomeExpenses : public Tab {
	Core* core;

	MonthlyTransactions_p monthlyTransactions;
	YearlyReport_p YearlyReport;

	//MonthlyAggrAccountReport_p monthlyAccountsReport;
	//MonthlyAggrSubCategoryReport_p monthlySubCategoryReport;
	//MonthlyAggrCategoryReport_p monthlyCategoryReport;
public:
	IncomeExpenses(Core* core);
	void Render();
	void ShowControlPanel(std::string panel_name);

	// Tabs
	void ShowIncomeExpensesAggregate();
	void IncomeExpenses::ShowIncomeExpensesDetails();
};