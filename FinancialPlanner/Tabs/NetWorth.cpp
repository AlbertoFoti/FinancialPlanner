#include "NetWorth.h"

//#define __TESTING_DEBUG__

NetWorth::NetWorth(Core* core) {
	this->core = core;

	core->ComputeNWdata();
}

void NetWorth::Render()
{
	ImGuiIO& io = ImGui::GetIO();
	auto blenderProHeavy_l = io.Fonts->Fonts[2];
	auto blenderProThin_m = io.Fonts->Fonts[7];
	auto blenderProThinLarge = io.Fonts->Fonts[8];

#ifdef __TESTING_DEBUG__
	static std::string test_string = "";
	if (ImGui::Button("Backend Test")) {
		test_string = this->core->testBackend();
	}
	ImGui::Text(test_string.c_str());
#endif

	//ImPlot::ShowDemoWindow();

	static std::vector<double> dates;
	static std::vector<double> opens;
	static std::vector<double> lows;
	static std::vector<double> highs;
	static std::vector<double> closes;

	this->NW_records = this->core->getNWdata();

	ImGui::Spacing();
	ImGui::PushFont(blenderProHeavy_l);
	ImGui::Text("Current Net Worth : "); ImGui::SameLine();
	ImGui::PopFont();
	ImGui::PushFont(blenderProThinLarge);
	if (NW_records[NW_records.size() - 1]->ClosingWorth > 10000)
		ImGui::Text("%.2fk EUR", NW_records[NW_records.size() - 1]->ClosingWorth/1000);
	else
		ImGui::Text("%.2f EUR", NW_records[NW_records.size() - 1]->ClosingWorth);
	ImGui::PopFont();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

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

	ImGui::SameLine();
	// Control (to)
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

	ImGui::SameLine();

	if (ImGui::Button("Refresh Data")) {
		this->NW_records = this->core->getNWdataFromDb(fromMSMtoUNIXtime(from), fromMSMtoUNIXtime(to));
	}

	ImGui::Separator();

	dates.clear();
	opens.clear();
	lows.clear();
	highs.clear();
	closes.clear();

	for (NW_record_p x : this->NW_records) {
		dates.push_back(getUNIXtime(x->Month, x->Year));
		opens.push_back(x->OpeningWorth);
		lows.push_back(x->LowWorth);
		highs.push_back(x->HighWorth);
		closes.push_back(x->ClosingWorth);
	}

	Plotter pl;
	pl.ShowLinePlot_def("##Net Worth (monthly)", &dates[0], &closes[0], dates.size());
	pl.ShowCandleBarsPlot_default("##Net Worth (candles)", &dates[0], &opens[0], &closes[0], &lows[0], &closes[0], dates.size());
	

	ShowControlPanel("Net Worth Control Panel");
}

void NetWorth::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());

	static bool byMonth = true;
	ImGui::Checkbox("Show by Month", &byMonth);

	ImGuiIO& io = ImGui::GetIO();
	auto blenderProThin_m = io.Fonts->Fonts[7];
	ImGui::PushFont(blenderProThin_m);

	ImGui::Separator();
	ImGui::Spacing();

	if (ImGui::BeginTable("netWorthTable", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
	{
		double delta;
		double delta_perc;

		ImVec4 color_positive = ImVec4(0.000f, 1.000f, 0.441f, 1.000f); // green
		ImVec4 color_negative = ImVec4(0.853f, 0.050f, 0.310f, 1.000f); // red

		for (int i = 0; i != this->NW_records.size(); ++i) {
			if (byMonth) {
				ImGui::TableNextColumn();
				ImGui::Text("%2d/%4d", NW_records[i]->Month, NW_records[i]->Year);
				ImGui::TableNextColumn();
				ImGui::Text("%.2f", NW_records[i]->ClosingWorth);
				ImGui::TableNextColumn();
				if (i == 0) {
					ImGui::Text("  /  ", NW_records[i]->ClosingWorth);
				}
				else {
					delta = NW_records[i]->ClosingWorth - NW_records[i - 1]->ClosingWorth;
					if (delta >= 0)
						ImGui::TextColored(color_positive, "+%.2f", delta);
					else
						ImGui::TextColored(color_negative, "%.2f", delta);
				}
				ImGui::TableNextColumn();
				if (i == 0) {
					ImGui::Text("  /  ", NW_records[i]->ClosingWorth);
				}
				else {
					delta_perc = (delta / NW_records[i-1]->ClosingWorth) * 100;
					if (delta_perc >= 0)
						ImGui::TextColored(color_positive, "+%.2f %%", delta_perc);
					else
						ImGui::TextColored(color_negative, "%.2f %%", delta_perc);
				}
			}
			else {
				if (NW_records[i]->Month == 12) {
					ImGui::TableNextColumn();
					ImGui::Text("%4d", NW_records[i]->Year);
					ImGui::TableNextColumn();
					ImGui::Text("%.2f", NW_records[i]->ClosingWorth);
					ImGui::TableNextColumn();
					if (i == 0) {
						ImGui::Text("  /  ", NW_records[i]->ClosingWorth);
					}
					else {
						delta = NW_records[i]->ClosingWorth - NW_records[i - 12]->ClosingWorth;
						if (delta >= 0)
							ImGui::TextColored(color_positive, "+%.2f", delta);
						else
							ImGui::TextColored(color_negative, "%.2f", delta);
					}
					ImGui::TableNextColumn();
					if (i == 0) {
						ImGui::Text("  /  ", NW_records[i]->ClosingWorth);
					}
					else {
						delta_perc = (delta / NW_records[i-12]->ClosingWorth) * 100;
						if (delta_perc >= 0)
							ImGui::TextColored(color_positive, "+%.2f %%", delta_perc);
						else
							ImGui::TextColored(color_negative, "%.2f %%", delta_perc);
					}
				}
			}
		}
		ImGui::EndTable();
	}

	ImGui::PopFont();

	ImGui::End();
}
