#include "Utility.h"

int getMSMtime(int month, int year)
{
	if (year >= 2000) {
		return (year - 2000) * 12 + month;
	}
	return 0;
}

int getMonthfromMSM(int msmDate)
{
	return ((msmDate % 12)+1);
}

int getYearfromMSM(int msmDate)
{
	return ((msmDate / 12) + 2000);
}

std::string getMonthfromMSM_s(int msmDate)
{
	return std::to_string((msmDate % 12)+1);
}

std::string getYearfromMSM_s(int msmDate)
{
	return std::to_string((msmDate / 12)+2000);
}

double getUNIXtime(int month, int year)
{
	time_t rawtime;
	struct tm timeinfo;

	/* get current timeinfo: */
	time(&rawtime); //or: rawtime = time(0);
	/* convert to struct: */
	localtime_s(&timeinfo, &rawtime);

	/* now modify the timeinfo to the given date: */
	timeinfo.tm_year = year - 1900;
	timeinfo.tm_mon = month - 1;    //months since January - [0,11]
	timeinfo.tm_mday = 1;          //day of the month - [1,31] 
	timeinfo.tm_hour = 0;         //hours since midnight - [0,23]
	timeinfo.tm_min = 0;          //minutes after the hour - [0,59]
	timeinfo.tm_sec = 0;          //seconds after the minute - [0,59]

	/* call mktime: create unix time stamp from timeinfo struct */
	return (double)mktime(&timeinfo);
}

double fromMSMtoUNIXtime(int msmDate)
{
	int month = getMonthfromMSM(msmDate);
	int year = getYearfromMSM(msmDate);

	return getUNIXtime(month, year);
}

Json::Value BubbleSortTransactions(Json::Value root)
{
	bool swapped = true;
	for (unsigned int i = 0; i != root["records"].size(); i++) {
		swapped = true;
		while (swapped) {
			swapped = false;
			for (unsigned int j = 0; j < root["records"][i]["data"].size() - 1; j++) {
				if (root["records"][i]["data"][j]["Day"] > root["records"][i]["data"][j + 1]["Day"]) {
					root = swapBubbleSortTransaction(root, i, j, j + 1);
					swapped = true;
				}
			}
		}
	}
	return root;
}

Json::Value swapBubbleSortTransaction(Json::Value root, int i, int j, int k) {
	Json::Value root2 = root["records"][i]["data"];
	Json::Value tmp = root2[j];
	root2[j] = root2[k];
	root2[k] = tmp;
	root["records"][i]["data"] = root2;

	return root;
}

Json::Value BubbleSortAccountDetails(Json::Value root)
{
	bool swapped = true;
	while (swapped) {
		swapped = false;
		for (unsigned int i = 0; i < root["records"].size() - 1; i++) {
			int monthXyear_i = root["records"][i]["Year"].asInt() * 12 + root["records"][i]["Month"].asInt();
			int monthXyear_next = root["records"][i + 1]["Year"].asInt() * 12 + root["records"][i + 1]["Month"].asInt();
			if (monthXyear_i > monthXyear_next) {
				root = swapBubbleSortAccountDetails(root, i, i + 1);
				swapped = true;
			}
		}
	};
	return root;
}

Json::Value swapBubbleSortAccountDetails(Json::Value root, int i, int j)
{
	Json::Value root2 = root["records"];
	Json::Value tmp = root2[i];
	root2[i] = root2[j];
	root2[j] = tmp;
	root["records"] = root2;

	return root;
}

Json::Value BubbleSortNetWorth(Json::Value root)
{
	bool swapped = true;
	while (swapped) {
		swapped = false;
		for (unsigned int i = 0; i < root["records"].size() - 1; i++) {
			int monthXyear_i = root["records"][i]["Year"].asInt() * 12 + root["records"][i]["Month"].asInt();
			int monthXyear_next = root["records"][i + 1]["Year"].asInt() * 12 + root["records"][i + 1]["Month"].asInt();
			if (monthXyear_i > monthXyear_next) {
				root = swapBubbleSortAccountDetails(root, i, i + 1);
				swapped = true;
			}
		}
	};
	return root;
}

Json::Value swapBubbleSortNetWorth(Json::Value root, int i, int j)
{
	Json::Value root2 = root["records"];
	Json::Value tmp = root2[i];
	root2[i] = root2[j];
	root2[j] = tmp;
	root["records"] = root2;

	return root;
}

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

	// Day
	int days = 31;
	if(month == 1) days = 29;
	if(month == 10 || month == 3 || month == 5 || month == 8) days = 30;
	static char selected[31] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for(int i=0; i<days; i++){
		if( i == 0 )
			ImGui::SetCursorPosX(window_pos);

		ImGui::PushID(i);
		if (ImGui::Selectable(std::to_string(i+1).c_str(), selected[i] != 0, 0, ImVec2(20, 20)))
		{
			selected[i] = 1;
			day = i+1;
			for(unsigned j = 0; j < days; j++){
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
