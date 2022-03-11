#include "Investments.h"

Investments::Investments(std::shared_ptr<Core> core)
{
	this->core = core;
}

void Investments::Render()
{

	// Tabs
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
	if (ImGui::BeginTabBar("IncomeExpensesTabs", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Overview"))
		{
			ShowInvestmentsOverview();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Breakdown"))
		{
			ShowInvestmentsBreakdown();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Details"))
		{
			ShowInvestmentsDetails();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	
	ShowControlPanel("Investments Control Panel");
}

void Investments::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());
	ImGui::Text(panel_name.c_str());
	ImGui::End();
}

void Investments::ShowInvestmentsOverview()
{
	Plotter pl;

	static std::vector<double> xs;
	static std::vector<double> ys;

	// Investments Line Plot
	std::shared_ptr<AccountMonthlyDetails> x = core->getAccountMonthlyRecords(1);
	static char str_line[100] = {};
	sprintf_s(str_line, "##%d_label_investments_line_plot", x->AccountID);
	for (int j = 0; j < x->accountMonthlyRecords.size(); j++) {
		int m = x->accountMonthlyRecords[j]->Month;
		int y = x->accountMonthlyRecords[j]->Year;
		double t = getUNIXtime(m, y);
		xs.push_back(t);
		ys.push_back(x->accountMonthlyRecords[j]->Amount);
	}
	if (xs.size() != 0 && ys.size() != 0) {
		pl.ShowLinePlot_def(str_line, &xs[0], &ys[0], (int)xs.size());
	}

	xs.clear();
	ys.clear();

	// Investments Candle plot
	static char str_candle[100] = {};
	sprintf_s(str_candle, "##%d_label_investments_candle_plot", x->AccountID);
	//pl.ShowCandleBarsPlot_default(str_candle, &xs[0], &ys[0], &ys[0], &ys[0], &ys[0], (int)xs.size());
}

void Investments::ShowInvestmentsBreakdown()
{
	Plotter pl;

	pl.ShowBarGroupsPlot_empty("Deposits");
	pl.ShowBarGroupsPlot_empty("Return on Investment");
}

void Investments::ShowInvestmentsDetails()
{
	ImGui::Text("Details...");
}