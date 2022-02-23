#include "NetWorth.h"

//#define __TESTING_DEBUG__

NetWorth::NetWorth(Core* core) {
	this->core = core;
}

void NetWorth::Render()
{
	ImGui::Text("Net Worth Tab");
	ImGui::Separator();
	
#ifdef __TESTING_DEBUG__
	static std::string test_string = "";
	if(ImGui::Button("Backend Test")) {
		test_string = this->core->testBackend();
	}
	ImGui::Text(test_string.c_str());
#endif

	ImPlot::ShowDemoWindow();

	static std::vector<double> dates;
	static std::vector<double> opens;
	static std::vector<double> lows;
	static std::vector<double> highs;
	static std::vector<double> closes;

	if (ImGui::Button("Refresh Net Worth Data")) {
		double from = getUNIXtime(1, 2019);
		double to = getUNIXtime(1, 2022);
		this->NW_records = this->core->getNWdataFromDb(from, to);
	}

	dates.clear();
	opens.clear();
	lows.clear();
	highs.clear();
	closes.clear();

	this->NW_records = this->core->getNWdata();
	for (NW_record_p x : this->NW_records) {
		dates.push_back(getUNIXtime(x->Month, x->Year));
		opens.push_back(x->OpeningWorth);
		lows.push_back(x->LowWorth);
		highs.push_back(x->HighWorth);
		closes.push_back(x->ClosingWorth);
	}

	
	Plotter pl;
	pl.ShowLinePlot_default("Net Worth (monthly)", &dates[0], &closes[0], dates.size());
	pl.ShowCandleBarsPlot_default("Net Worth Candle", &dates[0], &opens[0], &closes[0], &lows[0], &closes[0], dates.size());
	

	ShowControlPanel("Net Worth Control Panel");
}

void NetWorth::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());
	ImGui::Text("Control Panel");
	ImGui::End();
}
