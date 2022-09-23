#include "AssetAllocation.hpp"
#include "../Utility/Utility.hpp"

AssetAllocation::AssetAllocation(std::shared_ptr<Core> core)
{
	this->core = core;
}

void AssetAllocation::Render()
{

    // Tabs
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
	if (ImGui::BeginTabBar("AssetAllocationTabs", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Overview"))
		{
			ShowAssetAllocationOverview();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Breakdown"))
		{
			ShowAssetAllocationBreakdown();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Details"))
		{
			ShowAssetAllocationDetails();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	
	ShowControlPanel("AssetAllocation Control Panel");
}

void AssetAllocation::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());

	static bool byPercentage = true;
	
	Date_format_p x = std::make_shared<Date_format>();
	x->Day = 1;
	x->Month = 1;
	x->Year = 2000;

	static int year = 2021;

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
		}
	}
	ImGui::PopButtonRepeat();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Checkbox("Amount by Percentage (%)", &byPercentage);

	ImGui::Spacing();

	static std::vector<std::string> headerNames = { "Investment 1", "Investment 2", "Investment 3"};
	static std::vector<double> amounts_invested = {0.0, 0.0, 0.0};
	static std::vector<int> perc_invested = {0, 0, 0};
	for(int i=0; i<headerNames.size(); i++){
		if (ImGui::CollapsingHeader(headerNames[i].c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			AssetAllocationInvestment(i, byPercentage, amounts_invested, perc_invested);
		}
	}

	ImGui::Separator();
	ImGui::Spacing();

	static char name_new_investment[100] = "";
	ImGui::InputText("##name_new_investment", name_new_investment, IM_ARRAYSIZE(name_new_investment));

	// dims
    float win_dim_x = ImGui::GetWindowWidth();
    float win_dim_y = ImGui::GetWindowHeight();
    float dim_btn_small_x = win_dim_x*0.60f;
    float dim_btn_small_y = win_dim_y*0.03f;
    if(dim_btn_small_y < 50.0f) dim_btn_small_y = 50.0f;

	if(ImGui::Button("Add new investment", ImVec2(dim_btn_small_x, dim_btn_small_y))){
		// Add new investment
		headerNames.push_back(name_new_investment);
		amounts_invested.push_back(0.0);
		perc_invested.push_back(0);
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::Text("Month/Year : %d/%d", month, year);

	for(int i=0; i<amounts_invested.size(); i++){
		ImGui::Text("> %d : %.2f", i+1, amounts_invested.at(i));
	}
	for(int i=0; i<perc_invested.size(); i++){
		ImGui::Text("> %d : %d", i+1, perc_invested.at(i));
	}

	if(ImGui::Button("Add New Asset Allocation Record", ImVec2(dim_btn_small_x, dim_btn_small_y))){
		// Push new record if everything is ok
	}

	ImGui::End();
}

void AssetAllocation::ShowAssetAllocationOverview()
{
	ImGui::Text("Overview");
}

void AssetAllocation::ShowAssetAllocationBreakdown()
{
	ImGui::Text("Breakdown");
}

void AssetAllocation::ShowAssetAllocationDetails()
{
	ImGui::Text("Details");
}

void AssetAllocation::AssetAllocationInvestment(int i, bool byPercentage, std::vector<double>& amounts_invested, std::vector<int>& perc_invested)
{

	// dims
    float win_dim_x = ImGui::GetWindowWidth();
    float win_dim_y = ImGui::GetWindowHeight();
    float dim_btn_small_x = win_dim_x*0.20f;
    float dim_btn_small_y = win_dim_y*0.02f;
    if(dim_btn_small_y < 50.0f) dim_btn_small_y = 50.0f;

	// Amount of the total money invested dedicated to this single investment
	static char label_amount_invested[50] = "";
	sprintf(label_amount_invested, "of total portfolio##AmountInvested%d", i);
	if(byPercentage){
		ImGui::DragInt(label_amount_invested, &perc_invested.at(i), 1, 0, 100, "%d%%", ImGuiSliderFlags_AlwaysClamp);
	}else{
		ImGui::InputDouble(label_amount_invested, &amounts_invested.at(i), (0.0), (0.0), "%.2f");
	}

	// Edit and Delete Buttons aligned right
	ImVec2 buttonSize(dim_btn_small_x, dim_btn_small_y);
	float widthNeeded = buttonSize.x + buttonSize.x + ImGuiStyleVar_ItemSpacing;
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
	if (ImGui::Button("Edit##Edit_Button_Investment", buttonSize)) {

	}
	ImGui::SameLine();
	char strBtnDeleteInvestmentLabel[50] = {};
	sprintf(strBtnDeleteInvestmentLabel, "Delete##Del_Button_Investment%d", i + 1);
	if (ImGui::Button(strBtnDeleteInvestmentLabel, buttonSize)) {

	}
}