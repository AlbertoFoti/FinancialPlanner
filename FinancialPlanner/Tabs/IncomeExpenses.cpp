#include "IncomeExpenses.h"

IncomeExpenses::IncomeExpenses(Core* core) {
	this->core = core;
}

void IncomeExpenses::Render()
{
	// Import
	this->monthlyTransactions = this->core->getMonthlyTransactionsReport();
	this->YearlyReport = this->core->getYearlyReport();

	// Tabs
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
	if (ImGui::BeginTabBar("IncomeExpensesTabs", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Aggregate"))
		{
			ShowIncomeExpensesAggregate();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Details"))
		{
			ShowIncomeExpensesDetails();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ShowControlPanel("Income/Expenses Control Panel");
}

void IncomeExpenses::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());
	ImGui::Text("Control Panel : Income/Expenses");
	ImGui::End();
}

void IncomeExpenses::ShowIncomeExpensesAggregate() 
{
	ImGui::Text("Income / Expenses : Aggregate");
}

void IncomeExpenses::ShowIncomeExpensesDetails() 
{
	ImGuiIO& io = ImGui::GetIO();
	auto blenderProThin_m = io.Fonts->Fonts[7];

	ImVec4 color_positive = ImVec4(0.000f, 1.000f, 0.441f, 1.000f); // green
	ImVec4 color_negative = ImVec4(0.853f, 0.050f, 0.310f, 1.000f); // red

	enum Element { elem_January, elem_February, elem_March, elem_April, elem_May, elem_June, elem_July, elem_August, elem_September, elem_October, elem_November, elem_December, elem_Count };
	static int month = elem_January;
	const char* elems_names[elem_Count] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
	const char* elem_name = (month >= 0 && month < elem_Count) ? elems_names[month] : "Unknown";

	enum Option { elem_Month, elem_SubCategory, elem_Category, elem_Account, option_Count };
	static int breakdownDy = elem_Month;
	const char* option_names[option_Count] = { "by Month", "by SubCategory", "by Category", "by Account" };
	const char* option_name = (breakdownDy >= 0 && breakdownDy < option_Count) ? option_names[breakdownDy] : "Unknown";

	static int year = 2021;
	ImGui::SliderInt("##slider_year", &year, 2000, 2100);
	ImGui::SameLine();

	static bool monthlyAggrView = false;
	ImGui::Checkbox("Show Yearly", &monthlyAggrView);

	if (!monthlyAggrView) {
		ImGui::SliderInt("##slider_months", &month, 0, elem_Count - 1, elem_name);
		ImGui::SameLine();
	}
	else {
		ImGui::SliderInt("##slider_options", &breakdownDy, 0, option_Count - 1, option_name);
		ImGui::SameLine();
	}

	if (ImGui::Button("Refresh transactions"))
	{
		// Get Transactions
		if (!monthlyAggrView) {
			this->monthlyTransactions = this->core->getMonthlyTransactionsReportFromDb(month + 1, year);
		}
		else {
			if (breakdownDy == elem_Month) {
				this->YearlyReport = this->core->getYearlyReportFromDb(year);
			}
		}
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::PushFont(blenderProThin_m);

	if (!monthlyAggrView) {
		this->monthlyTransactions = this->core->getMonthlyTransactionsReportFromDb(month + 1, year);
		if (ImGui::BeginTable("IncExpTable", 5, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
		{
			for (int i = 0; i != this->monthlyTransactions->transactions.size(); ++i) {
				ImGui::TableNextColumn();
				ImGui::Text("%2d/%2d/%4d", monthlyTransactions->transactions[i]->Day, monthlyTransactions->Month, monthlyTransactions->Year);
				ImGui::TableNextColumn();
				ImGui::Text("%s", monthlyTransactions->transactions[i]->Type.c_str());
				ImGui::TableNextColumn();
				ImGui::Text("%s", monthlyTransactions->transactions[i]->Category.c_str());
				ImGui::TableNextColumn();
				ImGui::Text("%s", monthlyTransactions->transactions[i]->Subcategory.c_str());
				ImGui::TableNextColumn();
				if (monthlyTransactions->transactions[i]->Type == "In")
					ImGui::TextColored(color_positive, "+%.2f", monthlyTransactions->transactions[i]->Amount);
				else if (monthlyTransactions->transactions[i]->Type == "Out")
					ImGui::TextColored(color_negative, "%.2f", monthlyTransactions->transactions[i]->Amount);
				else
					ImGui::Text("%s", monthlyTransactions->transactions[i]->Amount);
			}
			ImGui::EndTable();
		}
	}
	else {
		if (breakdownDy == elem_Month) {
			static double savings;
			static double savings_rate;

			this->YearlyReport = this->core->getYearlyReportFromDb(year);
			if (ImGui::BeginTable("IncExpTable_month", 5, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
			{
				for (int i = 0; i != this->monthlyTransactions->transactions.size(); ++i) {
					ImGui::TableNextColumn();
					ImGui::Text("%2d/%4d", YearlyReport->monthlyReports[i]->Month, YearlyReport->Year);
					ImGui::TableNextColumn();
					ImGui::TextColored(color_positive, "+%.2f", YearlyReport->monthlyReports[i]->balanceIn);
					ImGui::TableNextColumn();
					ImGui::TextColored(color_negative, "%.2f", YearlyReport->monthlyReports[i]->balanceOut);
					ImGui::TableNextColumn();
					savings = YearlyReport->monthlyReports[i]->balanceIn + YearlyReport->monthlyReports[i]->balanceOut;
					if(savings >= 0)
						ImGui::TextColored(color_positive, "+%.2f", savings);
					else
						ImGui::TextColored(color_negative, "%.2f", savings);
					ImGui::TableNextColumn();
					savings_rate = savings / YearlyReport->monthlyReports[i]->balanceIn * 100;
					if (savings_rate >= 0)
						ImGui::TextColored(color_positive, "+%.2f %%", savings_rate);
					else
						ImGui::TextColored(color_negative, "%.2f %%", savings_rate);

				}
				ImGui::EndTable();
			}
		}
		else if (breakdownDy == elem_SubCategory) {
			ImGui::Text("Sub Category");
		}
		else if (breakdownDy == elem_Category) {
			ImGui::Text("Category");
		}
		else if (breakdownDy == elem_Account) {
			ImGui::Text("Account");
		}
	}

	ImGui::PopFont();

	static double MonthlyTotalIn;
	static double MonthlyTotalOut;
	if (!monthlyAggrView) {
		MonthlyTotalIn = 0.0;
		MonthlyTotalOut = 0.0;
		for (Transaction_p x : this->monthlyTransactions->transactions) {
			if (x->Type == "In") MonthlyTotalIn += x->Amount;
			if (x->Type == "Out") MonthlyTotalOut += x->Amount;
		}
		ImGui::Text("Total Income   : %.2f", MonthlyTotalIn);
		ImGui::Text("Total Expenses : %.2f", MonthlyTotalOut);
	}
}