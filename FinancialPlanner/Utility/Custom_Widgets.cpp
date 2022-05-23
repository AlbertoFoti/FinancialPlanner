#include "Custom_Widgets.h"

#include "imgui.h"

Date_format_p calendarSelection()
{
	Date_format_p x = std::make_shared<Date_format>();
	x->Day = 1;
	x->Month = 1;
	x->Year = 2000;

	static int year = 2021;
	static int day = 1;

	enum Element { elem_January, elem_February, elem_March, elem_April, elem_May, elem_June, elem_July, elem_August, elem_September, elem_October, elem_November, elem_December, elem_Count };
	static int month = elem_January;
	const char* elems_names[elem_Count] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
	const char* elem_name = (month >= 0 && month < elem_Count) ? elems_names[month] : "Unknown";

	ImGui::BulletText("Date");

	// Year
	float w = ImGui::GetContentRegionAvail().x;
	ImGui::AlignTextToFramePadding();
	float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
	ImGui::SetCursorPosX(50.0f);
	ImGui::PushButtonRepeat(true);
	if (ImGui::ArrowButton("##from_btn_back_year", ImGuiDir_Left)) { 
		if(year > 2000)
			year--; 
	}
	ImGui::SameLine(0.0f, spacing);
	ImGui::SetCursorPosX(w/2 - 5.0f);
	ImGui::Text("%d", year);
	ImGui::SameLine(0.0f, spacing);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - 50.0f);
	if (ImGui::ArrowButton("##from_btn_to_year", ImGuiDir_Right)) { 
		if (year < 2200)
			year++;
	}
	ImGui::PopButtonRepeat();

	// Month
	w = ImGui::GetContentRegionAvail().x;
	ImGui::AlignTextToFramePadding();
	spacing = ImGui::GetStyle().ItemInnerSpacing.x;
	ImGui::SetCursorPosX(50.0f);
	ImGui::PushButtonRepeat(true);
	float window_pos = ImGui::GetCursorPosX();
	if (ImGui::ArrowButton("##from_btn_back_month", ImGuiDir_Left)) { 
		if(month > 0){
			month--; 
			day = 1;
		}
	}
	ImGui::SameLine(0.0f, spacing);
	ImGui::SetCursorPosX(w/2 - 10.0f);
	ImGui::Text("%s", elems_names[month]);
	ImGui::SameLine(0.0f, spacing);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - 50.0f);
	if (ImGui::ArrowButton("##from_btn_to_month", ImGuiDir_Right)) { 
		if (month < 11){
			month++;
			day = 1;
		}
	}
	ImGui::PopButtonRepeat();

	ImGui::Spacing();

	// Window width calculations
	float day_dim_x = (int)(ImGui::GetWindowWidth() * 0.13);
	float day_dim_y = (int)(ImGui::GetWindowHeight() * 0.02);

	// Day
	int days = 31;
	if(month == 1) days = 29;
	if(month == 10 || month == 3 || month == 5 || month == 8) days = 30;
	static char selected[31] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for(int i=0; i<days; i++){
		if( i == 0 )
			ImGui::SetCursorPosX(window_pos);

		ImGui::PushID(i);
		if (ImGui::Selectable(std::to_string(i+1).c_str(), selected[i] != 0, 0, ImVec2(day_dim_x, day_dim_y)))
		{
			selected[i] = 1;
			day = i+1;
			for(int j = 0; j < days; j++){
				if(j != i)
					selected[j] = 0;
			}
		}
        ImGui::PopID();

		if(i != 6 && i != 13 && i != 20 && i != 27 && i != (days-1))
			ImGui::SameLine();
		else
			ImGui::SetCursorPosX(window_pos);
	}

	ImGui::Separator();

	x->Day = day;
	x->Month = month;
	x->Year = year;
	return x;
}