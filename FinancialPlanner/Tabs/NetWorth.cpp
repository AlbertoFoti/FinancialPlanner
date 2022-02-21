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

	//ImPlot::ShowDemoWindow();

	time_t rawtime;
	struct tm* timeinfo;

	static ImVec4 bullCol = ImVec4(0.000f, 1.000f, 0.441f, 1.000f);
	static ImVec4 bearCol = ImVec4(0.853f, 0.050f, 0.310f, 1.000f);

	double opens[25]  =  { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25 };
	double lows[25]   =  { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25 };
	double highs[25]  =  { 2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26 };
	double closes[25] =  { 2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26 };

	double dates[25] = { 239,240,241,242,243,244,245,246,247,248,249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263};
	for (double& x : dates) {
		int year = 2000 + x / 12;
		x = x - (year-2000) * 12;
		int month = x;

		/* get current timeinfo: */
		time(&rawtime); //or: rawtime = time(0);
		/* convert to struct: */
		timeinfo = localtime(&rawtime);

		/* now modify the timeinfo to the given date: */
		timeinfo->tm_year = year - 1900;
		timeinfo->tm_mon = month - 1;    //months since January - [0,11]
		timeinfo->tm_mday = 1;          //day of the month - [1,31] 
		timeinfo->tm_hour = 0;         //hours since midnight - [0,23]
		timeinfo->tm_min = 0;          //minutes after the hour - [0,59]
		timeinfo->tm_sec = 0;          //seconds after the minute - [0,59]

		/* call mktime: create unix time stamp from timeinfo struct */
		x = (double) mktime(timeinfo); 
	}

	if (ImPlot::BeginPlot("Net Worth")) {
		ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_Time, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_RangeFit);
		ImPlot::SetupAxesLimits(1570000000, 1640000000, 1, 30);
		ImPlot::PlotLine("Net Worth", dates, closes, 25);
		ImPlot::EndPlot();
	}

	Plotter pl;
	pl.ShowCandleBarsPlot("Net Worth Candle", dates, opens, closes, lows, highs, 218, 0.25f, bullCol, bearCol, 1570000000, 1640000000, 1, 30);


	ShowControlPanel("Net Worth Control Panel");
}

void NetWorth::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());
	ImGui::Text("Control Panel");
	ImGui::End();
}
