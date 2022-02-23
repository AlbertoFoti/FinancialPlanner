#include "Overview.h"

Overview::Overview(Core* core)
{
	this->core = core;
}

void Overview::Render()
{
	ImGuiIO& io = ImGui::GetIO();
	auto blenderProHeavy_l = io.Fonts->Fonts[2];
	auto blenderProThin_m = io.Fonts->Fonts[7];
	auto blenderProThinLarge = io.Fonts->Fonts[8];

	this->NW_records = this->core->getNWdata();

	ImGui::Spacing();
	ImGui::PushFont(blenderProHeavy_l);
	ImGui::Text("Current Net Worth : "); ImGui::SameLine();
	ImGui::PopFont();
	ImGui::PushFont(blenderProThinLarge);
	if (NW_records[NW_records.size() - 1]->ClosingWorth > 10000)
		ImGui::Text("%.2fk EUR", NW_records[NW_records.size() - 1]->ClosingWorth / 1000);
	else
		ImGui::Text("%.2f EUR", NW_records[NW_records.size() - 1]->ClosingWorth);
	ImGui::PopFont();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	static std::vector<double> dates;
	static std::vector<double> closes;

	dates.clear();
	closes.clear();
	for (NW_record_p x : this->NW_records) {
		dates.push_back(getUNIXtime(x->Month, x->Year));
		closes.push_back(x->ClosingWorth);
	}
	Plotter pl;
	pl.ShowLinePlot_def("##Net Worth (monthly)", &dates[0], &closes[0], dates.size());

	ShowControlPanel("Overview Control Panel");
}

void Overview::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());
	ImGui::Text(panel_name.c_str());
	ImGui::End();
}
