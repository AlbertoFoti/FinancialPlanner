#include "Investments.h"

Investments::Investments(std::shared_ptr<Core> core)
{
	this->core = core;
}

void Investments::Render()
{
	this->yearlyInvestmentsReport = this->core->getYearlyInvestmentsReport();

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
		int m = x->accountMonthlyRecords.at(j)->Month;
		int y = x->accountMonthlyRecords.at(j)->Year;
		double t = getUNIXtime(m, y);
		xs.push_back(t);
		ys.push_back(x->accountMonthlyRecords.at(j)->Amount);
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
	// Fonts
	ImGuiIO& io = ImGui::GetIO();
	auto blenderProThin_m = io.Fonts->Fonts[7];

	ImVec4 color_positive = ImVec4(0.000f, 1.000f, 0.441f, 1.000f); // green
	ImVec4 color_negative = ImVec4(0.853f, 0.050f, 0.310f, 1.000f); // red

	enum Option { elem_five, elem_ten, elem_twentyfive, elem_fifty, option_Count };
	static int breakdownDy = elem_five;
	const char* option_names[option_Count] = { "5 Years", "10 Years", "25 Years", "50 Years" };
	const char* option_name = (breakdownDy >= 0 && breakdownDy < option_Count) ? option_names[breakdownDy] : "Unknown";

	static int year = 2021;
	static int old_year = 2021;
	old_year = year;

	ImGui::SliderInt("##slider_year", &year, 2000, 2100);
	ImGui::SameLine();

	if (year != old_year)
		this->yearlyInvestmentsReport = core->getYearlyInvestmentsReportFromDb(year);

	static bool monthlyAggrView = false;
	ImGui::Checkbox("Show by TimeFrame", &monthlyAggrView);

	if (monthlyAggrView) {
		ImGui::SliderInt("##slider_options", &breakdownDy, 0, option_Count - 1, option_name);
		ImGui::SameLine();
	}

	if (ImGui::Button("Refresh transactions"))
	{
		// Get Transactions
		if (!monthlyAggrView) {
			// get monthly investments records
			this->yearlyInvestmentsReport = this->core->getYearlyInvestmentsReportFromDb(year);
		}
		else {
			if (breakdownDy == elem_five) {
				// get yearly investments records
			}
		}
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::PushFont(blenderProThin_m);
	float min_row_height = 30.0f;

	if (!monthlyAggrView) {
		if (ImGui::BeginTable("IncExpTable", 8, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
		{
			for (int i = 0; i != this->yearlyInvestmentsReport->monthlyInvestmentsReports.size(); ++i) {
				ImGui::TableNextRow(0,min_row_height);
				double initial_capital = yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->initial_capital - yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->deposits;
				ImGui::TableNextColumn();
				ImGui::Text("%2d/%4d", yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->Month, yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->Year);
				ImGui::TableNextColumn();
				ImGui::Text("%.2f", initial_capital);
				ImGui::TableNextColumn();
				ImGui::Text("%.2f", yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->deposits);
				ImGui::TableNextColumn();
				ImGui::Text("%.2f", yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->investments_variation);
				ImGui::TableNextColumn();
				if (yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->investments_variation > 0.0)
					ImGui::TextColored(color_positive, "+%.2f", yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->investments_variation);
				else if (yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->investments_variation < 0.0)
					ImGui::TextColored(color_negative, "%.2f", yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->investments_variation);
				else
					ImGui::Text("%.2f", yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->investments_variation);
				ImGui::TableNextColumn();
				double delta = yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->investments_variation + yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->deposits;
				if (delta > 0.0)
					ImGui::TextColored(color_positive, "+%.2f", delta);
				else if (delta < 0.0)
					ImGui::TextColored(color_negative, "%.2f", delta);
				else
					ImGui::Text("%.2f", delta);
				ImGui::TableNextColumn();
				double delta_perc = 100 * delta / initial_capital;
				if (delta_perc > 0.0)
					ImGui::TextColored(color_positive, "+%.2f%%", delta_perc);
				else if (delta_perc < 0.0)
					ImGui::TextColored(color_negative, "%.2f%%", delta_perc);
				else
					ImGui::Text("%.2f", delta_perc);
				ImGui::TableNextColumn();
				double delta_net = yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->investments_variation;
				double delta_net_perc = 100 * delta_net / initial_capital;
				if (delta_net_perc > 0.0)
					ImGui::TextColored(color_positive, "+%.2f", delta_net_perc);
				else if (delta_net_perc < 0.0)
					ImGui::TextColored(color_negative, "%.2f", delta_net_perc);
				else
					ImGui::Text("%.2f", delta_net_perc);
			}
			ImGui::EndTable();
		}
	}
	else {
		if (breakdownDy == elem_five) {
			ImGui::Text("five years details");
			/*
			static double savings;
			static double savings_rate;

			this->YearlyReport = this->core->getYearlyReportFromDb(year);
			if (ImGui::BeginTable("IncExpTable_month", 5, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
			{
				for (int i = 0; i != this->YearlyReport->monthlyReports.size(); ++i) {
					ImGui::TableNextColumn();
					ImGui::Text("%2d/%4d", YearlyReport->monthlyReports[i]->Month, YearlyReport->Year);
					ImGui::TableNextColumn();
					ImGui::TextColored(color_positive, "+%.2f", YearlyReport->monthlyReports[i]->balanceIn);
					ImGui::TableNextColumn();
					ImGui::TextColored(color_negative, "%.2f", YearlyReport->monthlyReports[i]->balanceOut);
					ImGui::TableNextColumn();
					savings = YearlyReport->monthlyReports[i]->balanceIn + YearlyReport->monthlyReports[i]->balanceOut;
					if (savings >= 0)
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
			*/
		}
		else if (breakdownDy == elem_ten) {
			ImGui::Text("ten years details");
		}
		else if (breakdownDy == elem_twentyfive) {
			ImGui::Text("twenty years details");
		}
		else if (breakdownDy == elem_fifty) {
			ImGui::Text("fifty years details");
		}
	}

	ImGui::PopFont();
}