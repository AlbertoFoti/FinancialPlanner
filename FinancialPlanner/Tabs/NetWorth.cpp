#include "NetWorth.hpp"

NetWorth::NetWorth(std::shared_ptr<Core> core) {
	this->core = core;
}

void NetWorth::Render()
{
	// Fonts
	ImGuiIO& io = ImGui::GetIO();
	auto blenderProHeavy_xl = io.Fonts->Fonts[26];
	auto blenderProThin_xl = io.Fonts->Fonts[25];

	// Net Worth Data
	this->NW_records = this->core->getNWdata();

	ImGui::ShowDemoWindow();

	// Current Net Worth
	ImGui::Spacing();
	ImGui::PushFont(blenderProHeavy_xl);
	ImGui::Text("Current Net Worth : "); ImGui::SameLine();
	ImGui::PopFont();
	ImGui::PushFont(blenderProThin_xl);
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
	ImGui::Separator();

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

	// dims
    float win_dim_x = ImGui::GetWindowWidth();
    float win_dim_y = ImGui::GetWindowHeight(); 
    float dim_btn_big_x = win_dim_x*0.15f;
    float dim_btn_big_y = win_dim_y*0.01f;
    if(dim_btn_big_y < 50.0f) dim_btn_big_y = 50;

	// Refresh Data Button (from Database)
	ImGui::SameLine();
	if (ImGui::Button("Refresh Data", ImVec2(dim_btn_big_x, dim_btn_big_y))) {
		this->NW_records = this->core->getNWdataFromDb(fromMSMtoUNIXtime(from), fromMSMtoUNIXtime(to));
	}

	ImGui::Separator();

	// Net Worth Plots
	Plotter pl;
	std::vector<double> dates;
	std::vector<double> opens;
	std::vector<double> lows;
	std::vector<double> highs;
	std::vector<double> closes;

	if (NW_records.size() == 0) {
		// Default empty plot
		pl.ShowEmptyPlot("##Empty_plot_1");
		pl.ShowEmptyPlot("##Empty_plot_2");
	}
	else {
		// Fill vectors
		for (NW_record_p x : this->NW_records) {
			dates.push_back(getUNIXtime(x->Month, x->Year));
			opens.push_back(x->OpeningWorth);
			lows.push_back(x->LowWorth);
			highs.push_back(x->HighWorth);
			closes.push_back(x->ClosingWorth);
		}

		// Plots
		ImPlot::PushColormap(ImPlotColormap_Dark);
		pl.ShowLinePlot_def("##Net Worth (monthly)", &dates[0], &closes[0], (int)dates.size());
		ImPlot::PopColormap();
		pl.ShowCandleBarsPlot_default("##Net Worth (candles)", &dates[0], &opens[0], &closes[0], &lows[0], &highs[0], (int)dates.size());
	}

	// Control Panel rendering...
	ShowControlPanel("Net Worth Control Panel");
}

void NetWorth::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());

	// Display for months or for years
	static bool byMonth = true;
	ImGui::Checkbox("Show by Month", &byMonth);

	// Fonts
	//ImGuiIO& io = ImGui::GetIO();
	//auto robotoProThin_m = io.Fonts->Fonts[16];
	//ImGui::PushFont(robotoProThin_m);

	ImGui::Separator();
	ImGui::Spacing();

	// Table of net worth data
	if (ImGui::BeginTable("netWorthTable", 4, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
	{
		double delta;
		double delta_perc;

		ImVec4 color_positive = ImVec4(0.000f, 1.000f, 0.441f, 1.000f); // green
		ImVec4 color_negative = ImVec4(0.853f, 0.050f, 0.310f, 1.000f); // red

		// headers
		ImGui::TableSetupColumn("Date");
        ImGui::TableSetupColumn("Net Worth");
        ImGui::TableSetupColumn("Delta (abs)");
		ImGui::TableSetupColumn("Delta (%)");
        ImGui::TableHeadersRow();
		if(byMonth){
			for (int i = 0; i != this->NW_records.size(); ++i) {
				ImGui::TableNextColumn();
				ImGui::Text("%2d/%4d", NW_records.at(i)->Month, NW_records.at(i)->Year);
				ImGui::TableNextColumn();
				ImGui::Text("%.2f", NW_records.at(i)->ClosingWorth);
				ImGui::TableNextColumn();
				delta = NW_records.at(i)->ClosingWorth - NW_records.at(i)->OpeningWorth;
				if (delta >= 0)
					ImGui::TextColored(color_positive, "+%.2f", delta);
				else
					ImGui::TextColored(color_negative, "%.2f", delta);
				ImGui::TableNextColumn();
				delta_perc = (delta / NW_records.at(i)->OpeningWorth) * 100;
				if (delta_perc >= 0)
					ImGui::TextColored(color_positive, "+%.2f%%", delta_perc);
				else
					ImGui::TextColored(color_negative, "%.2f%%", delta_perc);
			}
		}else{
			int curr_year = -1;
			int begin = -1;
			int index = -1;
			for (int i = 0; i != this->NW_records.size(); ++i){
				if(curr_year == -1){
					curr_year = this->NW_records.at(i)->Year;
					begin = i;
				}else{
					if(curr_year == this->NW_records.at(i)->Year){
						index = i;
					}else{
						// Push in the table
						ImGui::TableNextColumn();
						ImGui::Text("%4d", curr_year);
						ImGui::TableNextColumn();
						ImGui::Text("%.2f", NW_records.at(index)->ClosingWorth);
						ImGui::TableNextColumn();
						delta = NW_records.at(index)->ClosingWorth - NW_records.at(begin)->OpeningWorth;
						if (delta >= 0)
							ImGui::TextColored(color_positive, "+%.2f", delta);
						else
							ImGui::TextColored(color_negative, "%.2f", delta);
						ImGui::TableNextColumn();
						delta_perc = (delta / NW_records.at(begin)->OpeningWorth) * 100;
						if (delta_perc >= 0)
							ImGui::TextColored(color_positive, "+%.2f%%", delta_perc);
						else
							ImGui::TextColored(color_negative, "%.2f%%", delta_perc);
						// Update Curr_year
						curr_year = this->NW_records.at(i)->Year;
						begin = i;
						index = i;
					}
				}
			}

			// push last element in the table
			ImGui::TableNextColumn();
			ImGui::Text("%4d", curr_year);
			ImGui::TableNextColumn();
			ImGui::Text("%.2f", NW_records.at(index)->ClosingWorth);
			ImGui::TableNextColumn();
			delta = NW_records.at(index)->ClosingWorth - NW_records.at(begin)->OpeningWorth;
			if (delta >= 0)
				ImGui::TextColored(color_positive, "+%.2f", delta);
			else
				ImGui::TextColored(color_negative, "%.2f", delta);
			ImGui::TableNextColumn();
			delta_perc = (delta / NW_records.at(begin)->OpeningWorth) * 100;
			if (delta_perc >= 0)
				ImGui::TextColored(color_positive, "+%.2f%%", delta_perc);
			else
				ImGui::TextColored(color_negative, "%.2f%%", delta_perc);
		}

		ImGui::EndTable();
	}

	//ImGui::PopFont();

	ImGui::End();
}
