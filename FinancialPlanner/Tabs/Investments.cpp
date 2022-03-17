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

	// Control Bar variables (from, to)
	static int from = getMSMtime(1, 2019); 
	static int to = getMSMtime(1, 2023);
	static std::string from_s;
	static std::string to_s;
	static int max = getMSMtime(1, 2200);
	static int from_btn_counter = from;
	static int to_btn_counter = to;

	// Control (from)
	ImGui::AlignTextToFramePadding();
	float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
	ImGui::PushButtonRepeat(true);
	if (ImGui::ArrowButton("##from_btn_back", ImGuiDir_Left)) { 
		if(from_btn_counter > 0)
			from_btn_counter--; 
	}
	ImGui::SameLine(0.0f, spacing);
	if (ImGui::ArrowButton("##from_btn_to", ImGuiDir_Right)) { 
		if (from_btn_counter < max && from_btn_counter < to_btn_counter)
			from_btn_counter++;
	}
	ImGui::PopButtonRepeat();
	from = from_btn_counter;

	ImGui::SameLine();
	from_s = getMonthfromMSM_s(from) + "/" + getYearfromMSM_s(from);
	to_s = getMonthfromMSM_s(to) + "/" + getYearfromMSM_s(to);
	ImGui::DragIntRange2("##FROM_TO", &from, &to, 1, 0, max, from_s.c_str(), to_s.c_str());
	from_btn_counter = from;
	to_btn_counter = to;

	// Control (to)
	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::PushButtonRepeat(true);
	if (ImGui::ArrowButton("##to_btn_back", ImGuiDir_Left)) { 
		if(to_btn_counter > 0 && to_btn_counter > from_btn_counter )
			to_btn_counter--; 
	}
	ImGui::SameLine(0.0f, spacing);
	if (ImGui::ArrowButton("##to_btn_to", ImGuiDir_Right)) { 
		if(to_btn_counter < max )
			to_btn_counter++; 
	}
	ImGui::PopButtonRepeat();
	to = to_btn_counter;

	ImGui::Separator();

	// Investments Line Plot
	std::shared_ptr<AccountMonthlyDetails> x = core->getAccountMonthlyRecords(1);
	static char str_line[100] = {};
	sprintf_s(str_line, "##%d_label_investments_line_plot", x->AccountID);
	for (int j = 0; j < x->accountMonthlyRecords.size(); j++) {
		int m = x->accountMonthlyRecords.at(j)->Month;
		int y = x->accountMonthlyRecords.at(j)->Year;
		double t_msm = getMSMtime(m, y);
		if(t_msm > from && t_msm <= (to+1)){
			double t = getUNIXtime(m, y);
			xs.push_back(t);
			ys.push_back(x->accountMonthlyRecords.at(j)->Amount);
		}
	}
	if (xs.size() != 0 && ys.size() != 0) {
		pl.ShowLinePlot_def(str_line, xs.data(), ys.data(), (int)xs.size());
	}

	xs.clear();
	ys.clear();

	// Investments Candle plot
	//static char str_candle[100] = {};
	//sprintf_s(str_candle, "##%d_label_investments_candle_plot", x->AccountID);
	//pl.ShowCandleBarsPlot_default(str_candle, &xs[0], &ys[0], &ys[0], &ys[0], &ys[0], (int)xs.size());

	int rows = 1;
	int cols = 2;
	if (ImPlot::BeginSubplots("##sub_plots_investments", rows, cols, ImVec2(-1, -1))) {
		static char str[150] = {};

		// Plot 1
		pl.ShowEmptyPlot("Plot 1 - Investments");

		// Plot 2
		pl.ShowEmptyPlot("Plot 2 - Investments");
	

		ImPlot::EndSubplots();
	}
}

void Investments::ShowInvestmentsBreakdown()
{
	Plotter pl;

	//pl.ShowBarGroupsPlot_empty("Deposits");
	//pl.ShowBarGroupsPlot_empty("Return on Investment");

	// Selector
	static ImVec4 color_positive = ImVec4(0.000f, 1.000f, 0.441f, 1.000f); // green
	static ImVec4 color_negative = ImVec4(0.853f, 0.050f, 0.310f, 1.000f); // red

	enum Option { elem_five, elem_ten, elem_twentyfive, elem_fifty, option_Count };
	static int breakdownDy = elem_five;
	const char* option_names[option_Count] = { "5 Years", "10 Years", "25 Years", "50 Years" };
	const char* option_name = (breakdownDy >= 0 && breakdownDy < option_Count) ? option_names[breakdownDy] : "Unknown";

	static int year = 2021;

	ImGui::SliderInt("##slider_year", &year, 2000, 2100);
	ImGui::SameLine();
	float mid_window = ImGui::GetCursorPosX();

	if (year != this->yearlyInvestmentsReport->Year)
		this->yearlyInvestmentsReport = core->getYearlyInvestmentsReportFromDb(year);

	static bool monthlyAggrView = false;
	ImGui::Checkbox("Show by TimeFrame", &monthlyAggrView);

	static bool cumulative = false;

	if (monthlyAggrView) {
		ImGui::SliderInt("##slider_options", &breakdownDy, 0, option_Count - 1, option_name);
		ImGui::SameLine();
	}else{
		ImGui::Checkbox("Cumulative (from beginning of the Year)", &cumulative);
		ImGui::SameLine();
        ImGui::SetCursorPosX(mid_window);
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


	// Investments Plots
	static std::vector<const char*> xs;
	static std::vector<double> ys;
	
	if (!monthlyAggrView){
		int rows = 2;
		int cols = 2;
		if (ImPlot::BeginSubplots("Investment Plots", rows, cols, ImVec2(-1, -1))) {
			static char str[150] = {};

			if(!cumulative){
				// Plot 1
				ys.clear();
				sprintf_s(str, "Investments Portfolio Growth (net)##%d_label_inv_plots", 0);
				for(auto x : this->yearlyInvestmentsReport->monthlyInvestmentsReports){
					ys.push_back(x->investments_variation);
				}
				if (ys.size() != 0) {
					pl.ShowBarGroupsPlot_default(str, ys.data(), (int)ys.size());	
				}

				// Plot 2
				ys.clear();
				sprintf_s(str, "Investments Portfolio Growth##%d_label_inv_plots", 1);
				for(auto x : this->yearlyInvestmentsReport->monthlyInvestmentsReports){
					ys.push_back(x->deposits + x->investments_variation);
				}
				if (ys.size() != 0) {
					pl.ShowBarGroupsPlot_default(str, ys.data(), (int)ys.size());	
				}

				// Plot 3
				ys.clear();
				sprintf_s(str, "Investments Portfolio Growth (net) [%%]##%d_label_inv_plots", 2);
				for(auto x : this->yearlyInvestmentsReport->monthlyInvestmentsReports){
					double initial_inv = x->initial_capital - x->deposits - x->investments_variation;
					ys.push_back(100 * x->investments_variation / initial_inv);
				}
				if (ys.size() != 0) {
					pl.ShowBarGroupsPlot_default(str, ys.data(), (int)ys.size());	
				}

				// Plot 4
				ys.clear();
				sprintf_s(str, "Investments Portfolio Growth [%%]##%d_label_inv_plots", 3);
				for(auto x : this->yearlyInvestmentsReport->monthlyInvestmentsReports){
					double initial_inv = x->initial_capital - x->deposits - x->investments_variation;
					ys.push_back(100 * (x->deposits + x->investments_variation) / initial_inv);
				}
				if (ys.size() != 0) {
					pl.ShowBarGroupsPlot_default(str, ys.data(), (int)ys.size());	
				}
			}
			else {
				// Plot 1
				ys.clear();
				sprintf_s(str, "Cumulative Investments Portfolio Growth (net)##%d_label_inv_plots", 0);
				int i = 0;
				for(auto x : this->yearlyInvestmentsReport->monthlyInvestmentsReports){
					if( i!= 0)
						ys.push_back(x->investments_variation + ys.at(i - 1));
					else
						ys.push_back(x->investments_variation);
					i++;
				}
				if (ys.size() != 0) {
					pl.ShowBarGroupsPlot_default(str, ys.data(), (int)ys.size());	
				}

				// Plot 2
				ys.clear();
				sprintf_s(str, "Cumulative Investments Portfolio Growth##%d_label_inv_plots", 1);
				i = 0;
				for(auto x : this->yearlyInvestmentsReport->monthlyInvestmentsReports){
					if( i!= 0)
						ys.push_back(x->deposits + x->investments_variation + ys.at(i - 1));
					else
						ys.push_back(x->deposits + x->investments_variation);
					i++;
				}
				if (ys.size() != 0) {
					pl.ShowBarGroupsPlot_default(str, ys.data(), (int)ys.size());	
				}

				// Plot 3
				ys.clear();
				sprintf_s(str, "Cumulative Investments Portfolio Growth (net) [%%]##%d_label_inv_plots", 2);
				double base_capital = 0.0;
				i = 0;
				for(auto x : this->yearlyInvestmentsReport->monthlyInvestmentsReports){
					double initial_inv = x->initial_capital - x->deposits - x->investments_variation;
					if( i!= 0){
						ys.push_back(100 * x->investments_variation / base_capital);
					} else {
						base_capital = initial_inv;
						ys.push_back(100 * x->investments_variation / initial_inv);
					}
					i++;
				}
				if (ys.size() != 0) {
					pl.ShowBarGroupsPlot_default(str, ys.data(), (int)ys.size());	
				}

				// Plot 4
				ys.clear();
				sprintf_s(str, "Cumulative Investments Portfolio Growth [%%]##%d_label_inv_plots", 3);
				base_capital = 0.0;
				i = 0;
				for(auto x : this->yearlyInvestmentsReport->monthlyInvestmentsReports){
					if(i == 0){
						double initial_inv = x->initial_capital - x->deposits - x->investments_variation;
						base_capital = initial_inv;
					}
					ys.push_back(100 * (x->deposits + x->investments_variation) / base_capital);
					i++;
				}
				if (ys.size() != 0) {
					pl.ShowBarGroupsPlot_default(str, ys.data(), (int)ys.size());	
				}
			}

			ImPlot::EndSubplots();
		}
	}
	else {
		if(breakdownDy == elem_five){
			ImGui::Text("Five years timeframe");
		}
		else if(breakdownDy == elem_ten){
			ImGui::Text("Ten years timeframe");
		}
		else if(breakdownDy == elem_twentyfive){
			ImGui::Text("Twentyfive years timeframe");
		}
		else if(breakdownDy == elem_fifty){
			ImGui::Text("Fifty years timeframe");
		}
	}
	
}

void Investments::ShowInvestmentsDetails()
{
	// Fonts
	//ImGuiIO& io = ImGui::GetIO();
	//auto blenderProThin_m = io.Fonts->Fonts[7];

	static ImVec4 color_positive = ImVec4(0.000f, 1.000f, 0.441f, 1.000f); // green
	static ImVec4 color_negative = ImVec4(0.853f, 0.050f, 0.310f, 1.000f); // red

	enum Option { elem_five, elem_ten, elem_twentyfive, elem_fifty, option_Count };
	static int breakdownDy = elem_five;
	const char* option_names[option_Count] = { "5 Years", "10 Years", "25 Years", "50 Years" };
	const char* option_name = (breakdownDy >= 0 && breakdownDy < option_Count) ? option_names[breakdownDy] : "Unknown";

	static int year = 2021;

	ImGui::SliderInt("##slider_year", &year, 2000, 2100);
	ImGui::SameLine();
	float mid_window = ImGui::GetCursorPosX();

	if (year != this->yearlyInvestmentsReport->Year)
		this->yearlyInvestmentsReport = core->getYearlyInvestmentsReportFromDb(year);

	static bool monthlyAggrView = false;
	ImGui::Checkbox("Show by TimeFrame", &monthlyAggrView);

	if (monthlyAggrView) {
		ImGui::SliderInt("##slider_options", &breakdownDy, 0, option_Count - 1, option_name);
		ImGui::SameLine();
	}else{
		ImGui::SetCursorPosX(mid_window);
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

	//ImGui::PushFont(blenderProThin_m);
	float min_row_height = 30.0f;

	if (!monthlyAggrView) {
		if (ImGui::BeginTable("IncExpTable", 7, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
		{
			// Columns
			ImGui::TableNextRow(0, 20.0f);
			ImGui::TableNextColumn();
			ImGui::Text("MONTH");
			ImGui::TableNextColumn();
			ImGui::Text("INITIAL NET WORTH");
			ImGui::TableNextColumn();
			ImGui::Text("DEPOSITS");
			ImGui::TableNextColumn();
			ImGui::Text("INVESTMENTS DELTA (abs)");
			ImGui::TableNextColumn();
			ImGui::Text("PORTFOLIO DELTA (abs)");
			ImGui::TableNextColumn();
			ImGui::Text("INVESTMENTS DELTA (%%)");
			ImGui::TableNextColumn();
			ImGui::Text("PORTFOLIO DELTA (%%)");
			for (int i = 0; i != this->yearlyInvestmentsReport->monthlyInvestmentsReports.size(); ++i) {
				ImGui::TableNextRow(0,min_row_height);
				double initial_capital = yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->initial_capital - yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->deposits - yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->investments_variation;
				ImGui::TableNextColumn();
				ImGui::Text("%2d/%4d", yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->Month, yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->Year);
				ImGui::TableNextColumn();
				ImGui::Text("%.2f", initial_capital);
				ImGui::TableNextColumn();
				ImGui::Text("%.2f", yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->deposits);
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
				double delta_net = yearlyInvestmentsReport->monthlyInvestmentsReports.at(i)->investments_variation;
				double delta_net_perc = 100 * delta_net / initial_capital;
				if (delta_net_perc > 0.0)
					ImGui::TextColored(color_positive, "+%.2f%%", delta_net_perc);
				else if (delta_net_perc < 0.0)
					ImGui::TextColored(color_negative, "%.2f%%", delta_net_perc);
				else
					ImGui::Text("%.2f", delta_net_perc);
				ImGui::TableNextColumn();
				double delta_perc = 100 * delta / initial_capital;
				if (delta_perc > 0.0)
					ImGui::TextColored(color_positive, "+%.2f%%", delta_perc);
				else if (delta_perc < 0.0)
					ImGui::TextColored(color_negative, "%.2f%%", delta_perc);
				else
					ImGui::Text("%.2f", delta_perc);
			}
			ImGui::EndTable();
		}
	}
	else {
		if (breakdownDy == elem_five) {
			ImGui::Text("five years details");
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

	//ImGui::PopFont();
}