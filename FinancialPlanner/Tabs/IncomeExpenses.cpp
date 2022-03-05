#include "IncomeExpenses.h"

IncomeExpenses::IncomeExpenses(Core* core) {
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

	// Add New Transaction
	ImGui::Begin(panel_name.c_str());
	ImGui::Text("Add new Transaction");

	ImGui::Separator();

	ImGui::BulletText("Date");

	static char year_s[50] = {};
	ImGui::InputTextWithHint("Year##Year_new_trans", "2021", year_s, IM_ARRAYSIZE(year_s));

	enum Element { elem_January, elem_February, elem_March, elem_April, elem_May, elem_June, elem_July, elem_August, elem_September, elem_October, elem_November, elem_December, elem_Count };
	static int month = elem_January;
	const char* elems_names[elem_Count] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
	const char* elem_name = (month >= 0 && month < elem_Count) ? elems_names[month] : "Unknown";

	static int day = 1;
	static int maxDays = 31;

	ImGui::SliderInt("Month##slider_month", &month, 0, elem_Count - 1, elem_name);
	
	if (month == elem_November || month == elem_April || month == elem_June || month == elem_September)
		maxDays = 30;
	else if (month == elem_February)
		maxDays = 29;
	else
		maxDays = 31;
	ImGui::SliderInt("Day##Day_ie_new", &day, 1, maxDays, "%d");

	static int type = 0;
	ImGui::RadioButton("Income", &type, 0); ImGui::SameLine();
	ImGui::RadioButton("Expense", &type, 1);

	ImGui::BulletText("Category");
	static int cat = 0;
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

	ImGui::Spacing();

	bool something_went_wrong = false;
	if (!strcmp(year_s, "")) something_went_wrong = true;
	if (!strcmp(amount_s, "")) something_went_wrong = true;

	//something_went_wrong = this->core->checkErrors(x->Category, x->Subcategory, x->Type, x->Amount, std::stoi(year_s));

	if (ImGui::Button("Add New Transaction")) {
		if (something_went_wrong) {
			// Throw Error
		}
		else {
			// Add new Transaction
			Transaction_p x = new Transaction();
			x->Day = day;
			x->Category = categories[cat]->Name;
			x->Subcategory = subcategories[subCat]->Name;
			x->Type = type == 0 ? "In" : "Out";
			x->Amount = std::stod(amount_s);
			x->AccountID = acc + 1;

			// Push transaction
			core->pushTransaction(month + 1, std::stoi(year_s), x);

			// Clean Input Fields
			month = elem_January;
			day = 1;
			type = 0;
			cat = 0;
			subCat = 0;
			sprintf_s(amount_s, "");
			sprintf_s(year_s, "");
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

void IncomeExpenses::ShowDeleteTransactionPanel(int i, int month, int year)
{
	ImGui::Begin("Delete Transaction");
	ImGui::Text("Delete %d", i);
	ImGui::End();
}

void IncomeExpenses::ShowIncomeExpensesAggregate()
{
	ImGui::Text("Income / Expenses : Overview");
}

void IncomeExpenses::ShowIncomeExpensesDetails() 
{
	// Fonts
	ImGuiIO& io = ImGui::GetIO();
	auto blenderProThin_m = io.Fonts->Fonts[7];

	ImVec4 color_positive = ImVec4(0.000f, 1.000f, 0.441f, 1.000f); // green
	ImVec4 color_negative = ImVec4(0.853f, 0.050f, 0.310f, 1.000f); // red

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

	ImGui::PushFont(blenderProThin_m);

	if (!monthlyAggrView) {
		this->monthlyTransactions = this->core->getMonthlyTransactionsReportFromDb(month + 1, year);
		if (ImGui::BeginTable("IncExpTable", 6, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
		{
			for (int i = 0; i != this->monthlyTransactions->transactions.size(); ++i) {
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
					ImGui::Text("%s", monthlyTransactions->transactions[i]->Amount);
				ImGui::TableNextColumn();

				static bool showEdit = false;
				static bool showDelete = false;
				if (ImGui::Button("Edit")) {
					// edit transaction i
					showEdit = true;
				}
				if (ImGui::Button("Delete")) {
					// delete transaction i
					showDelete = true;
				}
				if (showEdit) this->ShowEditTransactionPanel(i, month + 1, year);
				if (showDelete) this->ShowDeleteTransactionPanel(i, month + 1, year);
			}
			ImGui::EndTable();
		}
	}
	else {
		if (breakdownDy == elem_Month) {
			static double savings;
			static double savings_rate;

			this->YearlyReport = this->core->getYearlyReportFromDb(year);
			if (ImGui::BeginTable("IncExpTable_month", 5, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
			{
				for (int i = 0; i != this->YearlyReport->monthlyReports.size(); ++i) {
					ImGui::TableNextColumn();
					ImGui::Text("%2d/%4d", YearlyReport->monthlyReports[i]->Month, YearlyReport->Year);
					ImGui::TableNextColumn();
					ImGui::TextColored(color_positive, "+%.2f", YearlyReport->monthlyReports[i]->balanceIn);
					ImGui::TableNextColumn();
					ImGui::TextColored(color_negative, "%.2f", YearlyReport->monthlyReports[i]->balanceOut);
					ImGui::TableNextColumn();
					savings = YearlyReport->monthlyReports[i]->balanceIn + YearlyReport->monthlyReports[i]->balanceOut;
					if(savings >= 0)
						ImGui::TextColored(color_positive, "+%.2f", savings);
					else
						ImGui::TextColored(color_negative, "%.2f", savings);
					ImGui::TableNextColumn();
					savings_rate = savings / YearlyReport->monthlyReports[i]->balanceIn * 100;
					if (savings_rate >= 0)
						ImGui::TextColored(color_positive, "+%.2f %%", savings_rate);
					else
						ImGui::TextColored(color_negative, "%.2f %%", savings_rate);

				}
				ImGui::EndTable();
			}
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

	static double MonthlyTotalIn;
	static double MonthlyTotalOut;
	if (!monthlyAggrView) {
		MonthlyTotalIn = 0.0;
		MonthlyTotalOut = 0.0;
		for (Transaction_p x : this->monthlyTransactions->transactions) {
			if (x->Type == "In") MonthlyTotalIn += x->Amount;
			if (x->Type == "Out") MonthlyTotalOut += x->Amount;
		}
		ImGui::Text("Total Income   : %.2f", MonthlyTotalIn);
		ImGui::Text("Total Expenses : %.2f", MonthlyTotalOut);
	}
}