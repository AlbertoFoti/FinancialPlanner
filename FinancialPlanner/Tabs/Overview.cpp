#include "Overview.h"

Overview::Overview(Core* core)
{
	this->core = core;
}

void Overview::Render()
{
	// Plotter
	Plotter pl;

	// Fonts
	ImGuiIO& io = ImGui::GetIO();
	auto blenderProHeavy_l = io.Fonts->Fonts[2];
	auto blenderProThin_m = io.Fonts->Fonts[7];
	auto blenderProThinLarge = io.Fonts->Fonts[8];

	// NW data
	this->NW_records = this->core->getNWdata();

	// Current Net Worth
	ImGui::Spacing();
	ImGui::PushFont(blenderProHeavy_l);
	ImGui::Text("Current Net Worth : "); ImGui::SameLine();
	ImGui::PopFont();
	ImGui::PushFont(blenderProThinLarge);
	if (NW_records.size() == 0) {
		ImGui::Text("0.00 EUR");
	}
	else {
		if (NW_records[NW_records.size() - 1]->ClosingWorth > 10000)
			ImGui::Text("%.2fk EUR", NW_records[NW_records.size() - 1]->ClosingWorth / 1000);
		else
			ImGui::Text("%.2f EUR", NW_records[NW_records.size() - 1]->ClosingWorth);
	}
	ImGui::PopFont();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// New Worth Plot
	static std::vector<double> dates;
	static std::vector<double> closes;

	dates.clear();
	closes.clear();

	if (NW_records.size() == 0) {
		// Default empty plot
		pl.ShowEmptyPlot("##Empty_plot_overview");
	}
	else {
		for (NW_record_p x : this->NW_records) {
			dates.push_back(getUNIXtime(x->Month, x->Year));
			closes.push_back(x->ClosingWorth);
		}

		ImPlot::PushColormap(ImPlotColormap_Dark);
		pl.ShowLinePlot_def("##Net Worth (monthly)", &dates[0], &closes[0], (int)dates.size());
		ImPlot::PopColormap();
	}

	// Account Plots
	static std::vector<double> xs;
	static std::vector<double> ys;
	
	int rows = (int)std::ceil(core->getAccountsSize() / 2.0);
	int cols = 2;
	if (ImPlot::BeginSubplots("Accounts", rows, cols, ImVec2(-1, -1))) {
		for (int i = 0; i < core->getAccountsSize(); ++i) {
			xs.clear();
			ys.clear();
			this->accountMonthlyRecords = core->getAccountMonthlyRecords(core->getIDfromIndex(i));
			char str[100] = {};
			sprintf_s(str, "%s##%d_label_account_plots", core->getAccountName(accountMonthlyRecords->AccountID).c_str(), accountMonthlyRecords->AccountID);
			for (int j = 0; j < accountMonthlyRecords->accountMonthlyRecords.size(); j++) {
				int m = accountMonthlyRecords->accountMonthlyRecords[j]->Month;
				int y = accountMonthlyRecords->accountMonthlyRecords[j]->Year;
				double t = getUNIXtime(m, y);
				xs.push_back(t);
				ys.push_back(accountMonthlyRecords->accountMonthlyRecords[j]->Amount);
			}
			if (xs.size() != 0 && ys.size() != 0) {
				pl.ShowLinePlot_def(str, &xs[0], &ys[0], (int)xs.size());
			}
		}
		ImPlot::EndSubplots();
	}
	


	ShowControlPanel("Overview Control Panel");
}

void Overview::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());
	ImGui::Text(panel_name.c_str());
	ImGui::End();
}
