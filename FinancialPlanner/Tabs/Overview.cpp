#include "Overview.hpp"

Overview::Overview(std::shared_ptr<Core> core)
{
	this->core = core;
}

void Overview::Render()
{
	// Plotter
	Plotter pl;

	// Fonts
	ImGuiIO& io = ImGui::GetIO();
	auto blenderProHeavy_xl = io.Fonts->Fonts[26];
	auto blenderProThin_xl = io.Fonts->Fonts[25];

	// NW data
	this->NW_records = this->core->getNWdata();

	// Current Net Worth
	ImGui::Spacing();
	ImGui::PushFont(blenderProHeavy_xl);
	ImGui::Text("Current Net Worth : "); ImGui::SameLine();
	ImGui::PopFont();
	ImGui::PushFont(blenderProThin_xl);
	if (NW_records.empty()) {
		ImGui::Text("0.00 EUR");
	}
	else {
		if (NW_records[NW_records.size() - 1]->ClosingWorth > 10000)
			ImGui::Text("%.2fk EUR", NW_records[NW_records.size() - 1]->ClosingWorth / 1000);
		else
			ImGui::Text("%.2f EUR", NW_records[NW_records.size() - 1]->ClosingWorth);
	}
	ImGui::PopFont();
	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// New Worth Plot
	static std::vector<double> dates;
	static std::vector<double> closes;
	

	dates.clear();
	closes.clear();

	if (NW_records.empty()) {
		// Default empty plot
		pl.ShowEmptyPlot("##Empty_plot_overview");
	}
	else {
		for (const NW_record_p& x : this->NW_records) {
			dates.push_back(getUNIXtime(x->Month, x->Year));
			closes.push_back(x->ClosingWorth);
		}

		ImPlot::PushColormap(ImPlotColormap_Dark);
		pl.ShowLinePlot_def("##Net Worth (monthly)", dates.data(), closes.data(), (int)dates.size());
		ImPlot::PopColormap();
	}

	// Account Plots
	static std::vector<double> xs;
	static std::vector<double> ys;
	
	int rows = (int)std::ceil(core->getAccountsSize() / 2.0);
	int cols = 2;
    if( rows == 0 ) rows = 1;
	if (ImPlot::BeginSubplots("Accounts", rows, cols, ImVec2(-1, -1))) {
		for (int i = 0; i < core->getAccountsSize(); ++i) {
			xs.clear();
			ys.clear();
			this->accountMonthlyRecords = core->getAccountMonthlyRecords(core->getIDfromIndex(i));
			char str[100] = {};
			sprintf(str, "%s##%d_label_account_plots", core->getAccountName(accountMonthlyRecords->AccountID).c_str(), accountMonthlyRecords->AccountID);
			for (auto & accountMonthlyRecord : accountMonthlyRecords->accountMonthlyRecords) {
				int m = accountMonthlyRecord->Month;
				int y = accountMonthlyRecord->Year;
				double t = getUNIXtime(m, y);
				xs.push_back(t);
				ys.push_back(accountMonthlyRecord->Amount);
			}
			if (!xs.empty() && !ys.empty()) {
                pl.ShowLinePlot_def(str, xs.data(), ys.data(), (int)xs.size());
			}
		}
		ImPlot::EndSubplots();
	}

	this->ShowAccountManager();
}

void Overview::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());
	ImGui::Text("%s", panel_name.c_str());
	ImGui::End();
}

void Overview::ShowAccountManager()
{
    // Fonts
    ImGuiIO& io = ImGui::GetIO();
    auto robotoProThin_m = io.Fonts->Fonts[22];
    auto robotoProThin_l = io.Fonts->Fonts[23];

    // Begin
    ImGui::Begin("Account Manager");

    // Retrieve accounts
    this->accounts = core->getAccounts();

    // dims
    float win_dim_x = ImGui::GetWindowWidth();
    float win_dim_y = ImGui::GetWindowHeight();
    float dim_btn_big_x = win_dim_x*0.30f;
    float dim_btn_big_y = win_dim_y*0.05f;
    float dim_btn_small_x = win_dim_x*0.30f;
    float dim_btn_small_y = win_dim_y*0.03f;
    if(dim_btn_big_y < 50.0f) dim_btn_big_y = 50;
    if(dim_btn_small_y < 50.0f) dim_btn_small_y = 50;

    ImGui::Spacing();
    if(ImGui::Button("Reload Accounts", ImVec2(dim_btn_big_x, dim_btn_big_y))) {
        this->accounts = this->core->getAccountsFromDb();
    }
    ImGui::Separator();

    // New Account Form
    ImGui::BulletText("Account Name");
    static char account_name[50] = {};
    ImGui::InputTextWithHint("##AN", "Bank/Cash/Investments/Savings", account_name, IM_ARRAYSIZE(account_name));

    if (ImGui::Button("Add Account", ImVec2(dim_btn_small_x, dim_btn_small_y))) {

        if (strcmp(account_name, "") != 0) {
            // New Account instance
            Account_p x = std::make_shared<Account>();
            x->id = accounts[accounts.size() - 1]->id + 1;
            x->name = account_name;
            x->totAmountStored = 0.0;

            // Todo: write in json file database
            this->core->pushAccount(x);

            // Clean input fields
            sprintf(account_name, "%s", "");

            // Update GUI
            this->accounts = core->getAccountsFromDb();
        }
        else {
            ImGui::OpenPopup("Something went wrong");
        }
    }
    if (ImGui::BeginPopupModal("Something went wrong", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Some input fields are invalid.\nCheck input fields and calculate again!\n\n");
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }
    ImGui::Separator();

    ImGui::TextUnformatted("Accounts");
    ImGui::Separator();

    // List of all accounts
    /*
    for (int i = 0; i < this->accounts.size(); i++) {
        ImGui::Text("%d. ", i+1); ImGui::SameLine();
        ImGui::Text("%s", accounts.at(i)->name.c_str());

        for (int j = 0; j < accounts.at(i)->sub_accounts.size(); j++) {
            ImGui::Text("%d. ", j+1); ImGui::SameLine();
            ImGui::Text("%s", accounts.at(i)->sub_accounts.at(j)->name.c_str());
            ImGui::PushFont(robotoProThin_m);
            if (accounts.at(i)->sub_accounts.at(j)->AmountStored >= 1000)
                ImGui::Text("%.2fk EUR", accounts.at(i)->sub_accounts.at(j)->AmountStored / 1000);
            else
                ImGui::Text("%.2f EUR", accounts.at(i)->sub_accounts.at(j)->AmountStored);
            ImGui::PopFont();
        }

        // Edit and Delete Buttons aligned right
        if(accounts.at(i)->id != 1){
            ImGui::SameLine();
            ImVec2 buttonSize(115.f, 0.f);
            float widthNeeded = buttonSize.x + buttonSize.x + ImGuiStyleVar_ItemSpacing;
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
            char strBtnUpdateLabel[50] = {};
            sprintf(strBtnUpdateLabel, "Update##Put_Button%d", i + 1);
            if (ImGui::Button(strBtnUpdateLabel, buttonSize)) {
                // Edit Account
            }
            ImGui::SameLine();
            char strBtnDeleteLabel[50] = {};
            sprintf(strBtnDeleteLabel, "Delete##Del_Button%d", i + 1);
            if (ImGui::Button(strBtnDeleteLabel, buttonSize)) {
                // Delete Account
                this->core->deleteAccount(accounts[i]->id);
            }
        }

        ImGui::Text("- "); ImGui::SameLine();

        ImGui::PushFont(robotoProThin_l);
        if (accounts.at(i)->totAmountStored >= 1000)
            ImGui::Text("%.2fk EUR", accounts.at(i)->totAmountStored / 1000);
        else
            ImGui::Text("%.2f EUR", accounts.at(i)->totAmountStored);
        ImGui::PopFont();
        ImGui::Separator();
    }
    */

    // List of all Accounts
    std::vector<std::string> headerNames;

    ImGui::PushFont(robotoProThin_m);
    for (int i = 0; i < this->accounts.size(); i++) {
        char str[70];
        sprintf(str, "%d. %s : %.2f", i+1, accounts.at(i)->name.c_str(), accounts.at(i)->totAmountStored);
        headerNames.push_back(str);

        if (ImGui::CollapsingHeader(headerNames[i].c_str(), ImGuiTreeNodeFlags_None))
        {
            ImGui::PopFont();
            for (auto & sub_account : accounts.at(i)->sub_accounts) {
                ImGui::Text("- "); ImGui::SameLine();
                ImGui::Text("%s", sub_account->name.c_str());
            }

            // Edit and Delete Buttons aligned right
            if(accounts.at(i)->id != 1){
                ImGui::SameLine();
                ImVec2 buttonSize(115.f, 0.f);
                float widthNeeded = buttonSize.x + buttonSize.x + ImGuiStyleVar_ItemSpacing;
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
                char strBtnUpdateLabel[50] = {};
                sprintf(strBtnUpdateLabel, "Update##Put_Button%d", i + 1);
                if (ImGui::Button(strBtnUpdateLabel, buttonSize)) {
                    // Edit Account
                }
                ImGui::SameLine();
                char strBtnDeleteLabel[50] = {};
                sprintf(strBtnDeleteLabel, "Delete##Del_Button%d", i + 1);
                if (ImGui::Button(strBtnDeleteLabel, buttonSize)) {
                    // Delete Account
                    this->core->deleteAccount(accounts[i]->id);
                }
            }
            ImGui::Separator();
            ImGui::PushFont(robotoProThin_m);
        }
    }
    ImGui::PopFont();

    ImGui::End();
}
