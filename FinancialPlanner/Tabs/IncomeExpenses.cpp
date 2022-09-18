#include "IncomeExpenses.hpp"

IncomeExpenses::IncomeExpenses(std::shared_ptr<Core> core) {
	this->core = core;
}

void IncomeExpenses::Render()
{
	// Import
	this->monthlyTransactions = this->core->getMonthlyTransactionsReport();
	this->YearlyReport = this->core->getYearlyReport();

	// Tabs
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
	if (ImGui::BeginTabBar("IncomeExpensesTabs", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Overview"))
		{
			ShowIncomeExpensesAggregate();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Details"))
		{
			ShowIncomeExpensesDetails();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ShowControlPanel("Income/Expenses Control Panel");
}

void IncomeExpenses::ShowControlPanel(std::string panel_name)
{
	// Data
	std::vector<Category_p> categories = this->core->getCategories();
	std::vector<SubCategory_p> subcategories;
	std::vector<Account_p> accounts;
	Date_format_p date;

	// Add New Transaction
	ImGui::Begin(panel_name.c_str());
	ImGui::Text("Add new Transaction");

	ImGui::Separator();

    if( categories.size() == 0 ) {
        ImGui::BulletText("No categories yet defined");
        ImGui::End();
        return;
    }

	date = calendarSelection();

	static int type = 0;
	ImGui::RadioButton("Income", &type, 0); ImGui::SameLine();
	ImGui::RadioButton("Expense", &type, 1); ImGui::SameLine();
	ImGui::RadioButton("Transfer", &type, 2);

	if(type != 2){
		ImGui::BulletText("Category");
		static int cat = 3;
		if (ImGui::BeginListBox("##cat_name_list_box"))
		{
			for (int n = 0; n < categories.size(); n++)
			{
				if (categories[n]->Type == (type == 0 ? "In" : "Out")) {
					const bool is_selected = (cat == n);

					if (ImGui::Selectable(categories[n]->Name.c_str(), is_selected))
						cat = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndListBox();
		}

		subcategories = this->core->getSubCategoriesOf(categories[cat]->Name);
		ImGui::BulletText("Sub Category");
		static int subCat = 0;
		if (ImGui::BeginListBox("##sub_cat_name_list_box"))
		{
			for (int n = 0; n < subcategories.size(); n++)
			{
				const bool is_selected = (subCat == n);
				if (ImGui::Selectable(subcategories[n]->Name.c_str(), is_selected))
					subCat = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}

		accounts = core->getAccounts();
		ImGui::BulletText("Account");
		static int acc = 0;
		if (ImGui::BeginListBox("##account_list_box"))
		{
			for (int n = 0; n < accounts.size(); n++)
			{
				const bool is_selected = (acc == n);
				if (ImGui::Selectable(accounts[n]->name.c_str(), is_selected))
					acc = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}

		static char amount_s[50] = {};
		ImGui::BulletText("Amount (in EUR) :");
		ImGui::InputTextWithHint("##Amount_new_trans", "300.00", amount_s, IM_ARRAYSIZE(amount_s));

		static char comment[35] = "";
		ImGui::BulletText("Note/Comment :");
		ImGui::InputText("##comment", comment, IM_ARRAYSIZE(comment));

		bool something_went_wrong = false;

		ImGui::Spacing();

		if (ImGui::Button("Add New Transaction")) {
			if (!strcmp(amount_s, "")){
				something_went_wrong = true;
			}else{
				something_went_wrong = this->core->checkErrors(cat, subCat, type == 0 ? "In" : "Out", std::stod(amount_s));
			}
			if (something_went_wrong) {
				// Throw Error
				ImGui::OpenPopup("Something went wrong");
			}
			else {
				// Add new Transaction
				Transaction_p x = std::make_shared<Transaction>();
				x->Day = date->Day;
				x->Category = categories[cat]->Name;
				x->Subcategory = subcategories[subCat]->Name;
				x->Type = type == 0 ? "In" : "Out";
				x->Amount = std::stod(amount_s);
				x->AccountID = acc + 1;
				x->accountTo = -1;
				x->Comment = comment;

				// Push transaction
				core->pushTransaction(date->Month + 1, date->Year, x);

				// Clean Input Fields
				sprintf(amount_s, "");
			}
		}

		if (ImGui::BeginPopupModal("Something went wrong", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Some input fields are invalid.\nCheck input fields and calculate again!\n\n");
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}
	}else{
		accounts = core->getAccounts();

		ImGui::BulletText("From");
		static int acc_from = 0;
		if (ImGui::BeginListBox("##account_list_box_from"))
		{
			for (int n = 0; n < accounts.size(); n++)
			{
				const bool is_selected = (acc_from == n);
				if (ImGui::Selectable(accounts[n]->name.c_str(), is_selected))
					acc_from = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}

		ImGui::BulletText("To");
		static int acc_to = 0;
		if (ImGui::BeginListBox("##account_list_box_to"))
		{
			for (int n = 0; n < accounts.size(); n++)
			{
				const bool is_selected = (acc_to == n);
				if (ImGui::Selectable(accounts[n]->name.c_str(), is_selected))
					acc_to = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}

		static char amount_s[50] = "";
		ImGui::BulletText("Amount (in EUR) :");
		ImGui::InputTextWithHint("##Amount_new_trans", "300.00", amount_s, IM_ARRAYSIZE(amount_s));

		static char comment[35] = "";
		ImGui::BulletText("Note/Comment :");
		ImGui::InputText("##comment", comment, IM_ARRAYSIZE(comment));
		
		bool something_went_wrong = false;

		ImGui::Spacing();
		
		if (ImGui::Button("Add New Transaction")) {
			if (!strcmp(amount_s, "")){
				something_went_wrong = true;
			}else{
				something_went_wrong = this->core->checkErrors(-3, 1, type == 0 ? "In" : (type == 1 ? "Out" : "Transfer"), std::stod(amount_s));
			}
			if (something_went_wrong) {
				// Throw Error
				ImGui::OpenPopup("Something went wrong");
			}
			else {
				// Add new Transaction
				Transaction_p x = std::make_shared<Transaction>();
				x->Day = date->Day;
				x->Category = "Transfer";
				x->AccountID = accounts[acc_from]->id;
				x->accountTo = accounts[acc_to]->id;
				if(x->accountTo == 1) x->Subcategory = "Deposit";
				else if(x->AccountID == 1) x->Subcategory = "Withdrawal";
				else x->Subcategory = "Transfer";
				x->Type = "Transfer";
				x->Amount = std::stod(amount_s);
				x->Comment = comment;

				// Push transaction
				core->pushTransaction(date->Month + 1, date->Year, x);

				// Clean Input Fields
				sprintf(amount_s, "");
			}
		}

		if (ImGui::BeginPopupModal("Something went wrong", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Some input fields are invalid.\nCheck input fields and calculate again!\n\n");
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}
	}

	ImGui::End();
}

void IncomeExpenses::ShowEditTransactionPanel(int i, int month, int year)
{
	ImGui::Begin("Edit Transaction");
	ImGui::Text("Edit %d", i);
	ImGui::End();
}

void IncomeExpenses::ShowIncomeExpensesAggregate()
{
	// Sliders and options
	enum Element { elem_January, elem_February, elem_March, elem_April, elem_May, elem_June, elem_July, elem_August, elem_September, elem_October, elem_November, elem_December, elem_Count };
	static int month = elem_January;
	const char* elems_names[elem_Count] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
	const char* elem_name = (month >= 0 && month < elem_Count) ? elems_names[month] : "Unknown";

	enum Option { elem_Category, elem_SubCategory, option_Count };
	static int breakdownBy = elem_Category;
	const char* option_names[option_Count] = { "by Category", "by SubCategory" };
	const char* option_name = (breakdownBy >= 0 && breakdownBy < option_Count) ? option_names[breakdownBy] : "Unknown";

	static int year = 2021;
	static int old_year = 2021;
	ImGui::SliderInt("##slider_year", &year, 2000, 2100);
	ImGui::SameLine();

	ImGui::AlignTextToFramePadding();
	float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
	ImGui::PushButtonRepeat(true);
	if (ImGui::ArrowButton("##btn_minus", ImGuiDir_Left)) { 
		if(year > 2000)
			year--; 
	}
	ImGui::SameLine(0.0f, spacing);
	if (ImGui::ArrowButton("##btn_plus", ImGuiDir_Right)) { 
		if (year < 2100)
			year++;
	}
	ImGui::PopButtonRepeat();
	ImGui::SameLine();

	static std::vector<MonthlyAggrCategoryReport_p> monthlyReportsYear;
	static bool includeInvestments = false;
	static bool old_include_investments = false;

	old_include_investments = includeInvestments;
	ImGui::Checkbox("Include Investments", &includeInvestments);
	if(old_include_investments != includeInvestments){
		monthlyReportsYear.clear();
		if (!includeInvestments) {
			for (int i = 0; i < 12; i++) {
				MonthlyAggrCategoryReport_p aggrCatReport = core->getAggrCatReportWithoutInvestments(i + 1, year);
				monthlyReportsYear.push_back(aggrCatReport);
			}
		}
		else {
			for (int i = 0; i < 12; i++) {
				MonthlyAggrCategoryReport_p aggrCatReport = core->getAggrCatReport(i + 1, year);
				monthlyReportsYear.push_back(aggrCatReport);
			}
		}

		old_include_investments = includeInvestments;
	}

	if(old_year != year){
		monthlyReportsYear.clear();
		if (!includeInvestments) {
			for (int i = 0; i < 12; i++) {
				MonthlyAggrCategoryReport_p aggrCatReport = core->getAggrCatReportWithoutInvestments(i + 1, year);
				monthlyReportsYear.push_back(aggrCatReport);
			}
		}
		else {
			for (int i = 0; i < 12; i++) {
				MonthlyAggrCategoryReport_p aggrCatReport = core->getAggrCatReport(i + 1, year);
				monthlyReportsYear.push_back(aggrCatReport);
			}
		}

		old_year = year;
	}

	ImGui::SliderInt("##slider_options", &breakdownBy, 0, option_Count - 1, option_name);
	ImGui::SameLine();

	static bool monthlyAggrView = false;
	ImGui::Checkbox("Show Yearly", &monthlyAggrView);
	ImGui::SameLine();

	static bool first_time = true;
	if (ImGui::Button("Refresh Data") || first_time) {
		monthlyReportsYear.clear();
		if (!includeInvestments) {
			for (int i = 0; i < 12; i++) {
				MonthlyAggrCategoryReport_p aggrCatReport = core->getAggrCatReportWithoutInvestments(i + 1, year);
				monthlyReportsYear.push_back(aggrCatReport);
			}
		}
		else {
			for (int i = 0; i < 12; i++) {
				MonthlyAggrCategoryReport_p aggrCatReport = core->getAggrCatReport(i + 1, year);
				monthlyReportsYear.push_back(aggrCatReport);
			}
		}
	}
	first_time = false;

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

    static bool normalize = true;

	if(!monthlyAggrView){
		static ImPlotSubplotFlags flags = ImPlotSubplotFlags_NoTitle || ImPlotSubplotFlags_NoResize;
		static int rows = 3;
		static int cols = 4;

		ImPlotStyle& style = ImPlot::GetStyle();
		ImVec2 old_legend_padding = style.LegendPadding;
		ImVec2 old_legend_inner_padding = style.LegendInnerPadding;
		ImVec2 old_legend_spacing = style.LegendSpacing;
		style.LegendPadding = ImVec2(0.0f, 0.0f);
		style.LegendInnerPadding = ImVec2(0.0f, 0.0f);
		style.LegendSpacing = ImVec2(0.0f, 0.0f);

		// months view
		if(breakdownBy == elem_Category){
			if (ImPlot::BeginSubplots("My Subplots", rows, cols, ImVec2(-1,-1), flags)) {
				ImPlotLegendFlags flags2 = ImPlotLegendFlags_None;
				static std::vector<const char*> labels;
				static std::vector<double> data;
				for (int i = 0; i < rows*cols; ++i) {
					ImPlot::PushColormap(ImPlotColormap_Deep);
					char str[100] = {};
					labels.clear();
					data.clear();
					for(auto x : monthlyReportsYear[i]->totalsByCategory){
						labels.push_back(x->Category.c_str());
						data.push_back(fabs(x->Amount));
					}
					sprintf(str, "%s##%d_label_pie_chart_cat", elems_names[i], i);
					if (ImPlot::BeginPlot(str, ImVec2(-1,-1), ImPlotFlags_Equal | ImPlotFlags_NoMouseText)) {
						ImPlot::SetupLegend(ImPlotLocation_West | ImPlotLocation_North, flags2);
						ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
						ImPlot::SetupAxesLimits(0, 1, 0, 1);
						ImPlot::PlotPieChart(labels.data(), data.data(), (int)labels.size(), 0.5, 0.5, 0.4, normalize, "%.0f");
						ImPlot::EndPlot();
					}
					ImPlot::PopColormap();
				}
				ImPlot::EndSubplots();
			}
		}else if(breakdownBy == elem_SubCategory){
			// sub cat monthly
		}

		style.LegendPadding = old_legend_padding;
		style.LegendInnerPadding = old_legend_inner_padding;
		style.LegendSpacing = old_legend_spacing;
	}
	else {
		// yearly aggregate view
		if(breakdownBy == elem_Category){
				ImPlot::PushColormap(ImPlotColormap_Deep);
				char str[100] = {};
				std::vector<const char*> labels;
				std::vector<double> data;
				for(int i=0; i<12; i++){
					for(auto cat : monthlyReportsYear[i]->totalsByCategory){
						bool found = false;
						for(int j=0; j<labels.size() && !found; j++){
							if(strcmp(labels[j], cat->Category.c_str()) == 0){
								found = true;
								data[j] += fabs(cat->Amount);
							}
						}
						if(!found){
							labels.push_back(cat->Category.c_str());
							data.push_back(fabs(cat->Amount));
						}
					}
				}
				if (ImPlot::BeginPlot("##label_pie_chart_cat_year", ImVec2(-1,-1), ImPlotFlags_Equal | ImPlotFlags_NoMouseText)) {
					ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
					ImPlot::SetupAxesLimits(0, 1, 0, 1);
					ImPlot::PlotPieChart(labels.data(), data.data(), (int)labels.size(), 0.5, 0.5, 0.35, normalize, "%.0f");
					ImPlot::EndPlot();
				}
				ImPlot::PopColormap();
		}else if(breakdownBy == elem_SubCategory){
			// sub cat yearly
		}
	}
}

void IncomeExpenses::ShowIncomeExpensesDetails() 
{
	// Fonts
	ImGuiIO& io = ImGui::GetIO();
	auto blenderProHeavy_l = io.Fonts->Fonts[15];
	auto blenderProThin_l = io.Fonts->Fonts[23];

	ImVec4 color_positive = ImVec4(0.000f, 1.000f, 0.441f, 1.000f); // green
	ImVec4 color_negative = ImVec4(0.853f, 0.050f, 0.310f, 1.000f); // red

	// dims
	float one_third_x = (int)(ImGui::GetWindowWidth()*0.33);

	// Sliders and options
	enum Element { elem_January, elem_February, elem_March, elem_April, elem_May, elem_June, elem_July, elem_August, elem_September, elem_October, elem_November, elem_December, elem_Count };
	static int month = elem_January;
	const char* elems_names[elem_Count] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
	const char* elem_name = (month >= 0 && month < elem_Count) ? elems_names[month] : "Unknown";

	enum Option { elem_Month, elem_SubCategory, elem_Category, elem_Account, option_Count };
	static int breakdownDy = elem_Month;
	const char* option_names[option_Count] = { "by Month", "by SubCategory", "by Category", "by Account" };
	const char* option_name = (breakdownDy >= 0 && breakdownDy < option_Count) ? option_names[breakdownDy] : "Unknown";

	static int year = 2021;
	ImGui::SliderInt("##slider_year", &year, 2000, 2100);
	ImGui::SameLine();

	static bool monthlyAggrView = false;
	ImGui::Checkbox("Show Yearly", &monthlyAggrView);

	if (!monthlyAggrView) {
		ImGui::SliderInt("##slider_months", &month, 0, elem_Count - 1, elem_name);
		ImGui::SameLine();
	}
	else {
		ImGui::SliderInt("##slider_options", &breakdownDy, 0, option_Count - 1, option_name);
		ImGui::SameLine();
	}

	if (ImGui::Button("Refresh transactions"))
	{
		// Get Transactions
		if (!monthlyAggrView) {
			this->monthlyTransactions = this->core->getMonthlyTransactionsReportFromDb(month + 1, year);
		}
		else {
			if (breakdownDy == elem_Month) {
				this->YearlyReport = this->core->getYearlyReportFromDb(year);
			}
		}
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::PushFont(blenderProThin_l);

	if (!monthlyAggrView) {
		this->monthlyTransactions = this->core->getMonthlyTransactionsReportFromDb(month + 1, year);

		// Transactions table
		ImGui::PushFont(blenderProHeavy_l);
		ImGui::Text("Transactions");
		ImGui::PopFont();
		ImGui::Spacing();
		if (ImGui::BeginTable("IncExpTable", 7, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			// headers
			ImGui::TableSetupColumn("DATE");
			ImGui::TableSetupColumn("TYPE");
			ImGui::TableSetupColumn("CATEGORY");
			ImGui::TableSetupColumn("SUBCATEGORY");
			ImGui::TableSetupColumn("AMOUNT");
			ImGui::TableSetupColumn("COMMENT");
			ImGui::TableSetupColumn("");
			ImGui::TableHeadersRow();
			for (int i = 0; i != this->monthlyTransactions->transactions.size(); ++i) {
				if(monthlyTransactions->transactions[i]->Type != "Transfer"){
					ImGui::TableNextColumn();
					ImGui::Text("%2d/%2d/%4d", monthlyTransactions->transactions[i]->Day, monthlyTransactions->Month, monthlyTransactions->Year);
					ImGui::TableNextColumn();
					ImGui::Text("%s", monthlyTransactions->transactions[i]->Type.c_str());
					ImGui::TableNextColumn();
					ImGui::Text("%s", monthlyTransactions->transactions[i]->Category.c_str());
					ImGui::TableNextColumn();
					ImGui::Text("%s", monthlyTransactions->transactions[i]->Subcategory.c_str());
					ImGui::TableNextColumn();
					if (monthlyTransactions->transactions[i]->Type == "In")
						ImGui::TextColored(color_positive, "+%.2f", monthlyTransactions->transactions[i]->Amount);
					else if (monthlyTransactions->transactions[i]->Type == "Out")
						ImGui::TextColored(color_negative, "%.2f", monthlyTransactions->transactions[i]->Amount);
					else
						ImGui::Text("%.2f", monthlyTransactions->transactions[i]->Amount);
					ImGui::TableNextColumn();
					ImGui::Text("%s", monthlyTransactions->transactions[i]->Comment.c_str());
					ImGui::TableNextColumn();
					static bool showEdit = false;
					if (ImGui::Button("Edit")) {
						// edit transaction i
						showEdit = true;
					}
					if (ImGui::Button("Delete")) {
						// delete transaction i
					}
					if (showEdit) this->ShowEditTransactionPanel(i, month + 1, year);
				}
			}
			ImGui::EndTable();
		}

		ImGui::Spacing();
		ImGui::PushFont(blenderProHeavy_l);
		ImGui::Text("Transfers");
		ImGui::PopFont();
		ImGui::Spacing();
		// Transfers table
		if (ImGui::BeginTable("IncExpTable_trasfers", 7, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			// headers
			ImGui::TableSetupColumn("DATE");
			ImGui::TableSetupColumn("TYPE");
			ImGui::TableSetupColumn("CATEGORY");
			ImGui::TableSetupColumn("SUBCATEGORY");
			ImGui::TableSetupColumn("AMOUNT");
			ImGui::TableSetupColumn("COMMENT");
			ImGui::TableSetupColumn("");
			ImGui::TableHeadersRow();
			for (int i = 0; i != this->monthlyTransactions->transactions.size(); ++i) {
				if(monthlyTransactions->transactions[i]->Type == "Transfer"){
					ImGui::TableNextColumn();
					ImGui::Text("%2d/%2d/%4d", monthlyTransactions->transactions[i]->Day, monthlyTransactions->Month, monthlyTransactions->Year);
					ImGui::TableNextColumn();
					ImGui::Text("%s", core->getAccountName(monthlyTransactions->transactions[i]->AccountID).c_str());
					ImGui::TableNextColumn();
					ImGui::Text("%s", core->getAccountName(monthlyTransactions->transactions[i]->accountTo).c_str());
					ImGui::TableNextColumn();
					ImGui::Text("%s", monthlyTransactions->transactions[i]->Subcategory.c_str());
					ImGui::TableNextColumn();
					ImGui::Text("%.2f", monthlyTransactions->transactions[i]->Amount);
					ImGui::TableNextColumn();
					ImGui::Text("%s", monthlyTransactions->transactions[i]->Comment.c_str());
					ImGui::TableNextColumn();

					static bool showEdit = false;
					if (ImGui::Button("Edit")) {
						// edit transaction i
						showEdit = true;
					}
					if (ImGui::Button("Delete")) {
						// delete transaction i
					}
					if (showEdit) this->ShowEditTransactionPanel(i, month + 1, year);
				}
			}
			ImGui::EndTable();
		}
		
		// Totals
		ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();
		static double MonthlyTotalIn;
		static double MonthlyTotalOut;
		static double MonthlyTotalInvVar;
		static double tot_save, net_save;
		ImGui::PushFont(blenderProHeavy_l);
		MonthlyTotalIn = 0.0;
		MonthlyTotalOut = 0.0;
		MonthlyTotalInvVar = 0.0;
		for (Transaction_p x : this->monthlyTransactions->transactions) {
			if (x->Type == "In") MonthlyTotalIn += x->Amount;
			if (x->Type == "Out") MonthlyTotalOut += x->Amount;
			if (x->Type == "Transfer" && x->Subcategory != "Transfer") MonthlyTotalInvVar += x->Amount;
		}
		ImGui::Text("Total Income   : "); ImGui::SameLine(one_third_x);
		ImGui::PushFont(blenderProThin_l);
		ImGui::Text("%.2f", MonthlyTotalIn);
		ImGui::PopFont();
		ImGui::Text("Total Expenses : "); ImGui::SameLine(one_third_x);
		ImGui::PushFont(blenderProThin_l);
		ImGui::Text("%.2f", MonthlyTotalOut);
		ImGui::PopFont();
		ImGui::Text("Total Investment Variation : "); ImGui::SameLine(one_third_x);
		ImGui::PushFont(blenderProThin_l);
		ImGui::Text("%.2f", MonthlyTotalInvVar);
		ImGui::PopFont();
		ImGui::Text("Total Savings : "); ImGui::SameLine(one_third_x);
		ImGui::PushFont(blenderProThin_l);
		tot_save = MonthlyTotalIn + MonthlyTotalOut + MonthlyTotalInvVar;
		ImGui::Text("%.2f", tot_save);
		ImGui::PopFont();
		ImGui::Text("Net Savings : "); ImGui::SameLine(one_third_x);
		ImGui::PushFont(blenderProThin_l);
		net_save = MonthlyTotalIn + MonthlyTotalOut;
		ImGui::Text("%.2f", (MonthlyTotalIn + MonthlyTotalOut));
		ImGui::PopFont();
		ImGui::Text("Total Savings Rate : "); ImGui::SameLine(one_third_x);
		ImGui::PushFont(blenderProThin_l);
		if(MonthlyTotalInvVar > 0)
			ImGui::Text("%.2f%%", (100 * tot_save / (MonthlyTotalIn + MonthlyTotalInvVar)));
		else 
			ImGui::Text("%.2f%%", (100 * tot_save / (MonthlyTotalIn)));
		ImGui::PopFont();
		ImGui::Text("Net Savings Rate : "); ImGui::SameLine(one_third_x);
		ImGui::PushFont(blenderProThin_l);
		ImGui::Text("%.2f%%", (100 * net_save / MonthlyTotalIn));
		ImGui::PopFont();
		ImGui::PopFont();
	}
	else {
		if (breakdownDy == elem_Month) {
			static double net_savings;
			static double savings;
			static double net_savings_rate;
			static double savings_rate;

			this->YearlyReport = this->core->getYearlyReportFromDb(year);
			if (ImGui::BeginTable("IncExpTable_month", 8, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
			{
				// headers
				ImGui::TableSetupColumn("DATE");
				ImGui::TableSetupColumn("BALANCE IN");
				ImGui::TableSetupColumn("INVESTMENTS DELTA");
				ImGui::TableSetupColumn("BALANCE OUT");
				ImGui::TableSetupColumn("NET SAVINGS");
				ImGui::TableSetupColumn("SAVINGS");
				ImGui::TableSetupColumn("NET SAVINGS RATE (%)");
				ImGui::TableSetupColumn("SAVINGS RATE (%)");
				ImGui::TableHeadersRow();
				for (int i = 0; i != this->YearlyReport->monthlyReports.size(); ++i) {
					ImGui::TableNextColumn();
					ImGui::Text("%2d/%4d", YearlyReport->monthlyReports[i]->Month, YearlyReport->Year);
					ImGui::TableNextColumn();
					ImGui::TextColored(color_positive, "+%.2f", YearlyReport->monthlyReports[i]->balanceIn);
					ImGui::TableNextColumn();
					if(YearlyReport->monthlyReports[i]->investmentsVariation >= 0)
						ImGui::TextColored(color_positive, "+%.2f", YearlyReport->monthlyReports[i]->investmentsVariation);
					else
						ImGui::TextColored(color_negative, "%.2f", YearlyReport->monthlyReports[i]->investmentsVariation);
					ImGui::TableNextColumn();
					ImGui::TextColored(color_negative, "%.2f", YearlyReport->monthlyReports[i]->balanceOut);
					ImGui::TableNextColumn();
					net_savings = YearlyReport->monthlyReports[i]->balanceIn + YearlyReport->monthlyReports[i]->balanceOut;
					if(net_savings >= 0)
						ImGui::TextColored(color_positive, "+%.2f", net_savings);
					else
						ImGui::TextColored(color_negative, "%.2f", net_savings);
					ImGui::TableNextColumn();
					savings = YearlyReport->monthlyReports[i]->balanceIn + YearlyReport->monthlyReports[i]->balanceOut + YearlyReport->monthlyReports[i]->investmentsVariation;
					if(savings >= 0)
						ImGui::TextColored(color_positive, "+%.2f", savings);
					else
						ImGui::TextColored(color_negative, "%.2f", savings);
					ImGui::TableNextColumn();
					net_savings_rate = net_savings / YearlyReport->monthlyReports[i]->balanceIn * 100;
					if (net_savings_rate >= 0)
						ImGui::TextColored(color_positive, "+%.2f %%", net_savings_rate);
					else
						ImGui::TextColored(color_negative, "%.2f %%", net_savings_rate);
					ImGui::TableNextColumn();
					if(YearlyReport->monthlyReports[i]->investmentsVariation > 0)
						savings_rate = savings / (YearlyReport->monthlyReports[i]->balanceIn + YearlyReport->monthlyReports[i]->investmentsVariation) * 100;
					else
						savings_rate = savings / (YearlyReport->monthlyReports[i]->balanceIn) * 100;
					if (savings_rate >= 0)
						ImGui::TextColored(color_positive, "+%.2f %%", savings_rate);
					else
						ImGui::TextColored(color_negative, "%.2f %%", savings_rate);

				}
				ImGui::EndTable();
			}

			// Totals
			ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing(); ImGui::Spacing();
			static double MonthlyTotalIn;
			static double MonthlyTotalOut;
			static double MonthlyTotalInvVar;
			static double tot_save;
			static double net_save;
			ImGui::PushFont(blenderProHeavy_l);
			MonthlyTotalIn = 0.0;
			MonthlyTotalOut = 0.0;
			MonthlyTotalInvVar = 0.0;
			for(auto x : YearlyReport->monthlyReports){
				MonthlyTotalIn += x->balanceIn;
				MonthlyTotalOut += x->balanceOut;
				MonthlyTotalInvVar += x->investmentsVariation;
			}
			ImGui::Text("Total Income   : "); ImGui::SameLine(one_third_x);
			ImGui::PushFont(blenderProThin_l);
			ImGui::Text("%.2f", MonthlyTotalIn);
			ImGui::PopFont();
			ImGui::Text("Total Expenses : "); ImGui::SameLine(one_third_x);
			ImGui::PushFont(blenderProThin_l);
			ImGui::Text("%.2f", MonthlyTotalOut);
			ImGui::PopFont();
			ImGui::Text("Total Investment Variation : "); ImGui::SameLine(one_third_x);
			ImGui::PushFont(blenderProThin_l);
			ImGui::Text("%.2f", MonthlyTotalInvVar);
			ImGui::PopFont();
			ImGui::Text("Total Savings : "); ImGui::SameLine(one_third_x);
			ImGui::PushFont(blenderProThin_l);
			tot_save = MonthlyTotalIn + MonthlyTotalOut + MonthlyTotalInvVar;
			ImGui::Text("%.2f", tot_save);
			ImGui::PopFont();
			ImGui::Text("Net Savings : "); ImGui::SameLine(one_third_x);
			ImGui::PushFont(blenderProThin_l);
			net_save = MonthlyTotalIn + MonthlyTotalOut;
			ImGui::Text("%.2f", (MonthlyTotalIn + MonthlyTotalOut));
			ImGui::PopFont();
			ImGui::Text("Total Savings Rate : "); ImGui::SameLine(one_third_x);
			ImGui::PushFont(blenderProThin_l);
			if(MonthlyTotalInvVar > 0)
				ImGui::Text("%.2f%%", (100 * tot_save / (MonthlyTotalIn + MonthlyTotalInvVar)));
			else 
				ImGui::Text("%.2f%%", (100 * tot_save / (MonthlyTotalIn)));
			ImGui::PopFont();
			ImGui::Text("Net Savings Rate : "); ImGui::SameLine(one_third_x);
			ImGui::PushFont(blenderProThin_l);
			ImGui::Text("%.2f%%", (100 * net_save / MonthlyTotalIn));
			ImGui::PopFont();
			ImGui::PopFont();
		}
		else if (breakdownDy == elem_SubCategory) {
			ImGui::Text("Sub Category");
		}
		else if (breakdownDy == elem_Category) {
			ImGui::Text("Category");
		}
		else if (breakdownDy == elem_Account) {
			ImGui::Text("Account");
		}
	}

	ImGui::PopFont();
}