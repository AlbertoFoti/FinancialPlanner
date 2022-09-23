#include "Manager.hpp"
#include "../Utility/namespace_declarations.hpp"

Manager::Manager(std::shared_ptr<Core> core)
{
    this->core = core;

    // Accounts
    this->accounts = this->core->getAccountsFromDb();
}

void Manager::Render()
{
    ImGui::Text("Manager...");

    this->ShowAccountManager();
    this->ShowCategoryManager();
    this->ShowInvCategoryManager();
    //this->ShowCompoundInterestCalculator("Compound Interest Calculator");

    this->ShowFontTesting();
    ImGui::ShowDemoWindow();
    ImPlot::ShowDemoWindow();
}

void Manager::ShowControlPanel(std::string panel_name)
{
    ImGui::Begin(panel_name.c_str());
    ImGui::Text(panel_name.c_str());
    ImGui::End();
}

void Manager::ShowAccountManager()
{
    // Fonts
    ImGuiIO& io = ImGui::GetIO();
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

        if (strcmp(account_name, "")) {
            // New Account instance
            Account_p x = std::make_shared<Account>();
            x->id = accounts[accounts.size() - 1]->id + 1;
            x->name = account_name;
            x->AmountStored = 0.0;

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
    if (ImGui::BeginPopupModal("Something went wrong", NULL, ImGuiWindowFlags_AlwaysAutoResize))
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
    for (int i = 0; i < this->accounts.size(); i++) {
        ImGui::Text("%d. ", i+1); ImGui::SameLine();
        ImGui::Text("%s", accounts.at(i)->name.c_str());

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
        if (accounts.at(i)->AmountStored >= 1000)
            ImGui::Text("%.2fk EUR", accounts.at(i)->AmountStored/1000);
        else
            ImGui::Text("%.2f EUR", accounts.at(i)->AmountStored);
        ImGui::PopFont();
        ImGui::Separator();
    }

    ImGui::End();
}

void Manager::ShowCategoryManager()
{
    // Begin
    ImGui::Begin("Category Manager");

    // Retrieve Categories
    this->categories = core->getCategories();

    // dims
    float win_dim_x = ImGui::GetWindowWidth();
    float win_dim_y = ImGui::GetWindowHeight();
    float dim_btn_big_x = win_dim_x*0.40f;
    float dim_btn_big_y = win_dim_y*0.05f;
    float dim_btn_small_x = win_dim_x*0.30f;
    float dim_btn_small_y = win_dim_y*0.03f;
    if(dim_btn_big_y < 50.0f) dim_btn_big_y = 50;
    if(dim_btn_small_y < 50.0f) dim_btn_small_y = 50;

    ImGui::Spacing();
    if (ImGui::Button("Reload Categories", ImVec2(dim_btn_big_x, dim_btn_big_y))) {
        this->categories = this->core->getCategoriesFromDb();
    }
    ImGui::Separator();

    // New Category or New Subcategory form
    static int newCat = 0;
    ImGui::RadioButton("Category", &newCat, 0); ImGui::SameLine();
    ImGui::RadioButton("Subcategory", &newCat, 1);

    static char category_name[50] = {};
    static char sub_category_name[50] = {};
    static char category_type[50] = {};

    if (newCat == 0) {
        ImGui::BulletText("Category Name");
        ImGui::InputTextWithHint("##MCN", "Housing / Health / Leisure", category_name, IM_ARRAYSIZE(category_name));

        ImGui::BulletText("Category Type");
        static int cat_type = 0;
        ImGui::RadioButton("Income", &cat_type, 0); ImGui::SameLine();
        ImGui::RadioButton("Expense", &cat_type, 1);
        if (cat_type == 0) {
            sprintf(category_type, "%s", "In");
        }
        else {
            sprintf(category_type, "%s", "Out");
        }

    }
    else {
        ImGui::BulletText("Category Name");
        ImGui::InputTextWithHint("##MCN", "Housing / Health / Leisure", category_name, IM_ARRAYSIZE(category_name));

        ImGui::BulletText("SubCategory Name");
        ImGui::InputTextWithHint("##SCN", "Rent / OtherHousing / Money", sub_category_name, IM_ARRAYSIZE(sub_category_name));
    }

    static char ButtonName[50] = {};
    if (newCat == 0) {
        sprintf(ButtonName, "%s", "Add Category");
    }
    else {
        sprintf(ButtonName, "%s", "Add Subcategory");
    }

    if (ImGui::Button(ButtonName, ImVec2(dim_btn_small_x, dim_btn_small_y))) {
        if (newCat == 0) {
            if (strcmp(category_name, "") && strcmp(category_type, "")) {
                // New Category instance
                Category_p x = std::make_shared<Category>();
                x->id = (int)categories.size() + 1;
                x->name = category_name;
                x->type = category_type;
                x->subCategories = std::vector<SubCategory_p>();

                SubCategory_p s = std::make_shared<SubCategory>();
                s->id = 1;
                char other[50] = "Other ";
                s->name = strcat(other, category_name);
                x->subCategories.push_back(s);

                if (!core->checkCategoryExists(x->name)) {// check not already in the category list
                    // write in json file database
                    this->core->pushCategory(x);
                }
                else {

                }

                // Clean input fields
                sprintf(category_name, "%s", "");
                sprintf(sub_category_name, "%s", "");

                // Reload GUI
                this->categories = core->getCategoriesFromDb();
            }
            else {
                ImGui::OpenPopup("Something went wrong");
            }
        }
        else {
            if (strcmp(category_name, "") && strcmp(sub_category_name, "")) {
                // New Subcategory instance
                SubCategory_p x = std::make_shared<SubCategory>();
                x->name = sub_category_name;

                // Push in category vector if category exists
                if (core->checkCategoryExists(category_name)) {
                    this->core->pushSubCategory(category_name, x);
                }
                else {
                    ImGui::OpenPopup("Something went wrong");
                }

                // Clean input fields
                sprintf(category_name, "%s", "");
                sprintf(sub_category_name, "%s", "");

                // Reload GUI
                this->categories = core->getCategoriesFromDb();
            }
            else {
                ImGui::OpenPopup("Something went wrong");
            }
        }
    }
    if (ImGui::BeginPopupModal("Something went wrong", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Some input fields are invalid.\nCheck input fields and calculate again!\n\n");
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }
    ImGui::Separator();

    ImGui::TextUnformatted("Categories");
    ImGui::Separator();

    // List of all Categories
    std::vector<std::string> headerNames;

    for (int i = 0; i < this->categories.size(); i++) {
        headerNames.push_back(std::to_string(i + 1) + ". " + categories[i]->name + " (" + (categories[i]->type == "In" ? "Income)" : (categories[i]->type == "Out") ? "Expense)" : "Transfer)"));

        if (ImGui::CollapsingHeader(headerNames[i].c_str(), ImGuiTreeNodeFlags_None))
        {
            for (int j = 0; j < categories[i]->subCategories.size(); j++) {
                ImGui::Text("- "); ImGui::SameLine();
                ImGui::Text("%s", categories[i]->subCategories[j]->name.c_str());
            }

            // Edit and Delete Buttons aligned right
            if(categories[i]->id != -3 && categories[i]->id != -2 && categories[i]->id != -1){
                ImVec2 buttonSize(115.f, 0.f);
                float widthNeeded = buttonSize.x + buttonSize.x + ImGuiStyleVar_ItemSpacing;
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x - widthNeeded);
                if (ImGui::Button("Edit##Edit_Button_Category", buttonSize)) {
                    // Edit Category i
                }
                ImGui::SameLine();
                char strBtnDeleteCatLabel[50] = {};
                sprintf(strBtnDeleteCatLabel, "Delete##Del_Button_Category%d", i + 1);
                if (ImGui::Button(strBtnDeleteCatLabel, buttonSize)) {
                    // Delete Account
                    this->core->deleteCategory(categories[i]->id);
                }
            }
        }
    }

    ImGui::End();
}

void Manager::ShowInvCategoryManager()
{
    // Begin
    ImGui::Begin("Investment Classes Manager");

    // Retrieve Investements Categories

    // dims
    float win_dim_x = ImGui::GetWindowWidth();
    float win_dim_y = ImGui::GetWindowHeight();
    float dim_btn_big_x = win_dim_x*0.50f;
    float dim_btn_big_y = win_dim_y*0.05f;
    float dim_btn_small_x = win_dim_x*0.50f;
    float dim_btn_small_y = win_dim_y*0.04f;
    if(dim_btn_big_y < 50.0f) dim_btn_big_y = 50;
    if(dim_btn_small_y < 50.0f) dim_btn_small_y = 50;

    ImGui::Spacing();
    if (ImGui::Button("Reload Investement Classes", ImVec2(dim_btn_big_x, dim_btn_big_y))) {
        // Retrieve Investements Categories from db
    }
    ImGui::Separator();

    // New Inv Class form

    static char category_name[50] = {};
    static char risk_profile_s[50] = {};

    ImGui::BulletText("Investment Class Name");
    ImGui::InputTextWithHint("##MCN", "Bond / Stock / Real Estate", category_name, IM_ARRAYSIZE(category_name));

    ImGui::BulletText("Risk profile");
    static int risk_profile = 0;
    ImGui::RadioButton("Low", &risk_profile, 0); ImGui::SameLine();
    ImGui::RadioButton("Medium", &risk_profile, 1); ImGui::SameLine();
    ImGui::RadioButton("High", &risk_profile, 2);
    if (risk_profile == 0) {
        sprintf(risk_profile_s, "%s", "Low");
    }else if (risk_profile == 1) {
        sprintf(risk_profile_s, "%s", "Medium");
    }else if (risk_profile == 2) {
        sprintf(risk_profile_s, "%s", "High");
    }

    static char ButtonName[50] = {};
    sprintf(ButtonName, "%s", "Add New Investment Class");

    if (ImGui::Button(ButtonName, ImVec2(dim_btn_small_x, dim_btn_small_y))) {
        if (strcmp(category_name, "")) {
            // New Category instance

            if (true) { // check not already in the category list
                // write in json file database
            }
            else {

            }

            // Clean input fields
            sprintf(category_name, "%s", "");

            // Reload GUI (from db)
        }
        else {
            ImGui::OpenPopup("Something went wrong");
        }
    }
    if (ImGui::BeginPopupModal("Something went wrong", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Some input fields are invalid.\nCheck input fields and calculate again!\n\n");
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }
    ImGui::Separator();

    ImGui::TextUnformatted("Investment Classes");
    ImGui::Separator();

    // List of all Investment classes


    ImGui::End();
}

void Manager::ShowCompoundInterestCalculator(const char *nameGUI)
{
    // create your GUI
    ImGui::Begin(nameGUI);

    // Basics ================================================
    ImGui::BulletText("Initial NW");
    static char initialNW[50] = {};
    ImGui::InputTextWithHint("##NW", "10000.00", initialNW, IM_ARRAYSIZE(initialNW));

    ImGui::BulletText("Expected interest rate (%%)");
    static char interestRate[50] = {};
    ImGui::InputTextWithHint("##IR", "10", interestRate, IM_ARRAYSIZE(interestRate));

    ImGui::BulletText("Annual Deposits");
    static char annualDeposits[50] = {};
    ImGui::InputTextWithHint("##AD", "1000", annualDeposits, IM_ARRAYSIZE(annualDeposits));

    ImGui::BulletText("Years of investment");
    static char investmentYears[50] = {};
    ImGui::InputTextWithHint("##YI", "10", investmentYears, IM_ARRAYSIZE(investmentYears));

    ImGui::Spacing();

    static double initialNW_d = 0.00;
    static double interestRate_d = 0.00;
    static int investmentYears_d = 0;
    static double annualDeposits_d = 0.00;

    static double NWendPeriod_d = 0.00;
    static double totalDeposits_d = 0.00;
    static double totalInterests_d = 0.00;

    static double x_data[100] = { 0 };
    for (int i = 0; i < 100; i++) {
        x_data[i] = (double)i;
    }
    static double y_data[100] = { 0 };

    if (ImGui::Button("Calculate")) {
        if (strcmp(initialNW, "") && strcmp(interestRate, "") && strcmp(annualDeposits, "") && strcmp(investmentYears, "")) {
            initialNW_d = std::stod(initialNW);
            interestRate_d = std::stod(interestRate);
            annualDeposits_d = std::stod(annualDeposits);
            investmentYears_d = std::stoi(investmentYears);
            NWendPeriod_d = this->core->CompoundInterestCalculate(initialNW_d, interestRate_d, annualDeposits_d, investmentYears_d, &y_data[0]);

            totalDeposits_d = initialNW_d + (annualDeposits_d * investmentYears_d);
            totalInterests_d = NWendPeriod_d - totalDeposits_d;
        }
        else {
            NWendPeriod_d = 0.00;
            totalDeposits_d = 0.00;
            totalInterests_d = 0.00;
            ImGui::OpenPopup("Something went wrong");
        }
    };

    if (ImGui::BeginPopupModal("Something went wrong", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Some input fields are invalid.\nCheck input fields and calculate again!\n\n");
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }


    // Calculation Result
    ImGui::BulletText("End of Period NW : ");
    static char NWendPeriod_s[50] = {};
    sprintf(NWendPeriod_s, "%.2f", NWendPeriod_d);
    ImGui::InputText("final NW", NWendPeriod_s, IM_ARRAYSIZE(NWendPeriod_s), ImGuiInputTextFlags_ReadOnly);

    ImGui::BulletText("Total Deposits   : ");
    static char totalDeposits[50] = {};
    sprintf(totalDeposits, "%.2f", totalDeposits_d);
    ImGui::InputText("total deposits", totalDeposits, IM_ARRAYSIZE(totalDeposits), ImGuiInputTextFlags_ReadOnly);

    ImGui::BulletText("Total Interests  : ");
    static char totalInterests[50] = {};
    sprintf(totalInterests, "%.2f", totalInterests_d);
    ImGui::InputText("total interests", totalInterests, IM_ARRAYSIZE(totalInterests), ImGuiInputTextFlags_ReadOnly);

    // Help Marker
    HelpMarker("Financial Planner - 2022 \n- initial NW : Net Worth (capital) at the beginning\n- interest Rate : return on investment in % (ex. 10%)\n- Years of investment: Years of compounding\n- Final NW: Net Worth (capital) after x years (deposits + interests)\n");

    // Compund Interest plot
    if (ImPlot::BeginPlot("Compound Interest Plot", NULL, NULL, ImVec2(-1, 0), ImPlotFlags_NoLegend | ImPlotFlags_NoBoxSelect | ImPlotFlags_AntiAliased, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_RangeFit, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_RangeFit)) {
        ImPlot::PushStyleColor(ImPlotCol_Line, ImVec4(1, 1, 0, 1));
        ImPlot::PushStyleColor(ImPlotCol_Fill, ImVec4(1, 1, 0, 1));
        ImPlot::PlotBars("Net Worth", y_data, investmentYears_d + 1);
        ImPlot::PopStyleColor();
        ImPlot::PopStyleColor();
        ImPlot::EndPlot();
    }

    ImGui::End();
}

void Manager::ShowFontTesting()
{
    ImGui::Begin("Font Demo Testing");

    ImGuiIO& io = ImGui::GetIO();

    auto blenderProHeavy_s = io.Fonts->Fonts[0];
    auto blenderProHeavy_m = io.Fonts->Fonts[1];
    auto blenderProHeavy_l = io.Fonts->Fonts[2];

    auto blenderProMedium_s = io.Fonts->Fonts[3];
    auto blenderProMedium_m = io.Fonts->Fonts[4];
    auto blenderProMedium_l = io.Fonts->Fonts[5];

    auto blenderProThin_s = io.Fonts->Fonts[6];
    auto blenderProThin_m = io.Fonts->Fonts[7];
    auto blenderProThin_l = io.Fonts->Fonts[8];

    ImGui::PushFont(blenderProHeavy_s);
    ImGui::Text("Blender Pro Heavy small");
    ImGui::PopFont();

    ImGui::PushFont(blenderProHeavy_m);
    ImGui::Text("Blender Pro Heavy medium");
    ImGui::PopFont();

    ImGui::PushFont(blenderProHeavy_l);
    ImGui::Text("Blender Pro Heavy large");
    ImGui::PopFont();

    ImGui::PushFont(blenderProMedium_s);
    ImGui::Text("Blender Pro Medium small");
    ImGui::PopFont();

    ImGui::PushFont(blenderProMedium_m);
    ImGui::Text("Blender Pro Medium medium");
    ImGui::PopFont();

    ImGui::PushFont(blenderProMedium_l);
    ImGui::Text("Blender Pro Medium large");
    ImGui::PopFont();

    ImGui::PushFont(blenderProThin_s);
    ImGui::Text("Blender Pro Thin small");
    ImGui::PopFont();

    ImGui::PushFont(blenderProThin_m);
    ImGui::Text("Blender Pro Thin medium");
    ImGui::PopFont();

    ImGui::PushFont(blenderProThin_l);
    ImGui::Text("Blender Pro Thin large");
    ImGui::PopFont();

    ImGui::End();
}