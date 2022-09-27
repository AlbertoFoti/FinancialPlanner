#include "Backend.hpp"
#include "../Utility/Utility.hpp"
#include "../Settings.hpp"

std::string Backend::root_directory_db = ROOT_DIRECTORY_DATABASE;
std::string Backend::root_fonts = ROOT_FONTS_CONFIG;

void Backend::init() {

}

std::vector<Account_p> Backend::getAccounts()
{
    Json::Value root;

    std::vector<Account_p> accounts;

    root = getRootFromFileStream(root_directory_db + "/accounts.json");
    
    Json::Value acc = root["accounts"];

    for (auto & i : acc) {
        Account_p x = std::make_shared<Account>();
        x->id = std::stoi(i["id"].asString());
        x->name = i["name"].asString();
        x->totAmountStored = 0.0;
        // calculate Amount stored by each account based on last report
        x->totAmountStored = this->getLastAccountAmount(x->id);

        Json::Value sub_acc = i["subAccounts"];
        for(auto & j : sub_acc) {
            SubAccount_p y = std::make_shared<SubAccount>();
            y->id = std::stoi(j["id"].asString());
            y->name = j["name"].asString();
            y->AmountStored = this->getLastSubAccountAmount(x->id, y->id);
            x->sub_accounts.push_back(y);
        }

        accounts.push_back(x);
    }

    return accounts;
}

void Backend::pushAccount(const Account_p& x)
{
    Json::Value root;

    root = getRootFromFileStream(root_directory_db + "/accounts.json");

    // push in accounts list
    unsigned int nextIndex = root["accounts"].size();
    root["accounts"][nextIndex]["id"] = x->id;
    root["accounts"][nextIndex]["name"] = x->name;

    // Write the output to a file
    writeToFileStream(root_directory_db + "/accounts.json", root);
}

AccountMonthlyDetails_p Backend::getAccountMonthlyRecords(int id)
{
    Json::Value root;

    double last_amount = 0.0;

    AccountMonthlyDetails_p x = std::make_shared<AccountMonthlyDetails>();
    x->AccountID = id;

    std::vector<Account_p> accounts;

    root = getRootFromFileStream(root_directory_db + "/accountsDetails.json");

    int last_month = -1;
    int last_year = -1;
    for (unsigned int i = 0; i < root["records"].size(); i++) {
        AccountMonthlyRecordComplex_p item = std::make_shared<AccountMonthlyRecordComplex>();
        item->Month = std::stoi(root["records"][i]["Month"].asString());
        item->Year = std::stoi(root["records"][i]["Year"].asString());
        item->Amount = 0.0;

        bool found = false;
        for (unsigned int j = 0; j < root["records"][i]["data"].size(); j++) {
            if (root["records"][i]["data"][j]["id"] == id) {
                found = true;
                item->Amount = std::stod(root["records"][i]["data"][j]["Amount"].asString());
                last_amount = item->Amount;
            }
        }

        if(found){
            if(i!=0){
                int diff = (item->Year*12+item->Month) - (last_year*12+last_month);
                if(diff > 1){
                    while(diff > 1){
                        AccountMonthlyRecordComplex_p j = std::make_shared<AccountMonthlyRecordComplex>();
                        if(last_month == 12){
                            j->Month = 1;
                            last_month = 1;
                            j->Year = last_year + 1;
                            last_year = last_year + 1;
                        }else{
                            j->Month = last_month + 1;
                            last_month = last_month + 1;
                            j->Year = last_year;
                        }
                        j->Amount = item->Amount;
                        x->accountMonthlyRecords.push_back(j);
                        diff--;
                    }
                }
            }
            last_month = item->Month;
            last_year = item->Year;
            x->accountMonthlyRecords.push_back(item);
        }else{
            // Padding an empty month
            item->Amount = last_amount;
            x->accountMonthlyRecords.push_back(item);
            last_month = item->Month;
            last_year = item->Year;
        }
    }

    return x;
}

AccountMonthlyDetails_p Backend::getAccountMonthlyRecordsComplete(int id)
{
    Json::Value root;

    AccountMonthlyDetails_p x = std::make_shared<AccountMonthlyDetails>();
    x->AccountID = id;

    std::vector<Account_p> accounts;

    root = getRootFromFileStream(root_directory_db + "/accountsDetails.json");

    for (unsigned int i = 0; i < root["records"].size(); i++) {
        AccountMonthlyRecordComplex_p item = std::make_shared<AccountMonthlyRecordComplex>();
        item->Month = std::stoi(root["records"][i]["Month"].asString());
        item->Year = std::stoi(root["records"][i]["Year"].asString());
        item->Amount = 0.0;

        for (unsigned int j = 0; j < root["records"][i]["data"].size(); j++) {
            if (root["records"][i]["data"][j]["id"] == id) {
                item->Amount = std::stod(root["records"][i]["data"][j]["Amount"].asString());
            }
        }
        x->accountMonthlyRecords.push_back(item);
    }

    return x;
}

void Backend::deleteAccount(int id)
{
    Json::Value root;

    root = getRootFromFileStream(root_directory_db + "/accounts.json");

    Json::Value accounts = root["accounts"];
    Json::Value newArrayAccounts = Json::arrayValue;

    for (auto & account : accounts) {
        if (account["id"].asInt() != id) {
            // push in new vector
            unsigned int newArraySize = newArrayAccounts.size();
            newArrayAccounts[newArraySize]["id"] = account["id"];
            newArrayAccounts[newArraySize]["name"] = account["name"];
        }
    }
    root["accounts"] = newArrayAccounts;

    // Write the output to a file
    writeToFileStream(root_directory_db + "/accounts.json", root);
}

std::vector<Category_p> Backend::getCategories()
{
    Json::Value root;

    std::vector<Category_p> categories;

    root = getRootFromFileStream(root_directory_db + "/categories.json");

    Json::Value cat = root["categories"];

    for (auto & i : cat) {
        Category_p x = std::make_shared<Category>();
        x->id = std::stoi(i["id"].asString());
        x->name = i["Name"].asString();
        x->type = i["Type"].asString();
        for (unsigned int j = 0; j < i["SubCategories"].size(); j++) {
            SubCategory_p s = std::make_shared<SubCategory>();
            s->id = std::stoi(i["SubCategories"][j]["id"].asString());
            s->name = i["SubCategories"][j]["Name"].asString();
            x->subCategories.push_back(s);
        }

        categories.push_back(x);
    }

    return categories;
}

void Backend::pushCategory(Category_p x)
{
    Json::Value root;

    root = getRootFromFileStream(root_directory_db + "/categories.json");

    unsigned int nextIndex = root["categories"].size();
    root["categories"][nextIndex]["id"] = x->id;
    root["categories"][nextIndex]["Name"] = x->name;
    root["categories"][nextIndex]["Type"] = x->type;
    for (int i = 0; i < x->subCategories.size(); i++) {
        root["categories"][nextIndex]["SubCategories"][i]["id"] = x->subCategories[i]->id;
        root["categories"][nextIndex]["SubCategories"][i]["Name"] = x->subCategories[i]->name;
    }

    // Write the output to a file
    writeToFileStream(root_directory_db + "/categories.json", root);
}

void Backend::pushSubCategory(const std::string& categoryName, SubCategory_p x)
{
    Json::Value root;
    int index = -1;

    root = getRootFromFileStream(root_directory_db + "/categories.json");

    for (unsigned int i = 0; i < root["categories"].size(); i++) {
        if (root["categories"][i]["Name"].asString() == categoryName) {
            index = i;
            int nextIndex = root["categories"][i]["SubCategories"].size();
            root["categories"][i]["SubCategories"][nextIndex]["id"] = nextIndex + 1;
            root["categories"][i]["SubCategories"][nextIndex]["Name"] = x->name;
        }
    }

    // Other + category name at the end of the list
    root = SwapLastElements(root, index);

    // Write the output to a file
    writeToFileStream(root_directory_db + "/categories.json", root);
}

Json::Value Backend::SwapLastElements(Json::Value root, int i)
{
    std::string tmp;
    unsigned int lastIndex = root["categories"][i]["SubCategories"].size();
    if(lastIndex == 0) return root;
    else {
        lastIndex -= 1;
    }

    tmp = root["categories"][i]["SubCategories"][lastIndex]["Name"].asString();
    root["categories"][i]["SubCategories"][lastIndex]["Name"] = root["categories"][i]["SubCategories"][lastIndex - 1]["Name"];
    root["categories"][i]["SubCategories"][lastIndex - 1]["Name"] = tmp;

    return root;
}

void Backend::deleteCategory(int id)
{
    Json::Value root;

    root = getRootFromFileStream(root_directory_db + "/categories.json");

    Json::Value categories = root["categories"];
    Json::Value newArrayCategories = Json::arrayValue;

    for (unsigned int i = 0; i < categories.size(); i++) {
        if (categories[i]["id"].asInt() != id) {
            // push in new vector
            int newArraySize = newArrayCategories.size();
            newArrayCategories[newArraySize] = categories[i];
        }
    }
    root["categories"] = newArrayCategories;

    // Write the output to a file
    writeToFileStream(root_directory_db + "/categories.json", root);
}

std::vector<NW_record_p> Backend::getNWdata(double from, double to)
{
    Json::Value root;

    std::vector<NW_record_p> NW_data;

    root = getRootFromFileStream(root_directory_db + "/netWorth.json");

    Json::Value data = root["records"];

    double last_amount = 0.0;
    int last_month = -1;
    int last_year = -1;

    for (unsigned int i = 0; i < data.size(); i++) {
        NW_record_p x = std::make_shared<NW_record>();
        x->Month = std::stoi(data[i]["Month"].asString());
        x->Year = std::stoi(data[i]["Year"].asString());
        x->OpeningWorth = std::stod(data[i]["OpeningWorth"].asString());
        x->LowWorth = std::stod(data[i]["LowWorth"].asString());
        x->HighWorth = std::stod(data[i]["HighWorth"].asString());
        x->ClosingWorth = std::stod(data[i]["ClosingWorth"].asString());

        double UnixTime = getUNIXtime(x->Month, x->Year);

        // Push
        if (from == -1 && to == -1) {
            if(i!=0){
                int diff = (last_year*12+last_month)-(x->Year*12+x->Month);
                if(diff > 1){
                    while(diff > 1){
                        NW_record_p y = std::make_shared<NW_record>();
                        if(last_month == 12){
                            y->Month = 1;
                            last_month = 1;
                            y->Year = last_year + 1;
                            last_year = last_year + 1;
                        }else{
                            y->Month = last_month + 1;
                            last_month = last_month + 1;
                            y->Year = last_year;
                        }
                        y->OpeningWorth = last_amount;
                        y->LowWorth = last_amount;
                        y->HighWorth = last_amount;
                        y->ClosingWorth = last_amount;

                        NW_data.push_back(y);

                        diff--;
                    }
                }
            }
            last_amount = x->ClosingWorth;
            last_month = x->Month;
            last_year = x->Year;

            NW_data.push_back(x);
        }
        else {
            if(i!=0){
                int diff = (x->Year*12+x->Month) - (last_year*12+last_month);
                if(diff > 1){
                    while(diff > 1){
                        NW_record_p y = std::make_shared<NW_record>();
                        if(last_month == 12){
                            y->Month = 1;
                            last_month = 1;
                            y->Year = last_year + 1;
                            last_year = last_year + 1;
                        }else{
                            y->Month = last_month + 1;
                            last_month = last_month + 1;
                            y->Year = last_year;
                        }
                        y->OpeningWorth = last_amount;
                        y->LowWorth = last_amount;
                        y->HighWorth = last_amount;
                        y->ClosingWorth = last_amount;

                        double UnixTime_y = getUNIXtime(y->Month, y->Year);

                        if(UnixTime_y >= from && UnixTime_y <= to)
                            NW_data.push_back(y);

                        diff--;
                    }
                }
            }
            last_amount = x->ClosingWorth;
            last_month = x->Month;
            last_year = x->Year;

            if(UnixTime >= from && UnixTime <= to)
                NW_data.push_back(x);
        }
    }

    return NW_data;
}

// Income / Expenses

MonthlyTransactions_p Backend::getMonthlyReport(int month, int year)
{
    Json::Value root;

    MonthlyTransactions_p MonthlyReport = std::make_shared<MonthlyTransactions>();
    MonthlyReport->Month = month;
    MonthlyReport->Year = year;
    MonthlyReport->transactions = std::vector<Transaction_p>();

    root = getRootFromFileStream(root_directory_db + "/incomeExpenses.json");

    Json::Value data = root["records"];
    Json::Value transactRecords;

    for (auto & i : data) {
        if (i["Month"].asInt() == month && i["Year"].asInt() == year) {
            MonthlyReport->Month = month;
            MonthlyReport->Year = year;
            transactRecords = i["data"];
            for (auto & transactRecord : transactRecords) {
                Transaction_p t = std::make_shared<Transaction>();
                t->day = transactRecord["Day"].asInt();
                t->category = transactRecord["Category"].asString();
                t->sub_category = transactRecord["Subcategory"].asString();
                t->type = transactRecord["Type"].asString();
                t->account_id = transactRecord["AccountFrom"].asInt();
                t->account_to = transactRecord["AccountTo"].asInt();
                t->amount = transactRecord["Amount"].asDouble();
                t->comment = transactRecord["Comment"].asString();
                MonthlyReport->transactions.push_back(t);
            }
        }
    }

    return MonthlyReport;
}

YearlyReport_p Backend::getYearlyReport(int year)
{
    YearlyReport_p yearlyReport = std::make_shared<YearlyReport>();
    yearlyReport->Year = year;
    yearlyReport->monthlyReports = std::vector<MonthlyReport_p>();

    for (int i = 0; i < 12; i++) {
        MonthlyTransactions_p monthlyReport = this->getMonthlyReport(i + 1, year);
        MonthlyReport_p x = std::make_shared<MonthlyReport>();
        x->Month = i + 1;
        x->Year = year;
        x->balanceIn = 0.0;
        x->balanceOut = 0.0;
        x->investmentsVariation = 0.0;
        for (const auto& t : monthlyReport->transactions) {
            if(t->category == "Investments (+)" || t->category == "Investments (-)"){
                if(t->sub_category == "Investments (+)" || t->category == "Investments (-)"){
                    x->investmentsVariation += t->amount;
                }
            }else{
                if (t->type == "In")
                    x->balanceIn += t->amount;
                if (t->type == "Out")
                    x->balanceOut += t->amount;
            }
        }

        yearlyReport->monthlyReports.push_back(x);
    }

    return yearlyReport;
}

void Backend::pushTransaction(int month, int year, Transaction_p t)
{
    Json::Value root;
    bool found = false;

    root = getRootFromFileStream(root_directory_db + "/incomeExpenses.json");

    for (unsigned int i = 0; i < root["records"].size(); i++) {
        if ((root["records"][i]["Year"] == year) && (root["records"][i]["Month"] == month)) {
            found = true;
            int index = root["records"][i]["data"].size();
            root["records"][i]["data"][index]["Day"] = t->day;
            root["records"][i]["data"][index]["Category"] = t->category;
            root["records"][i]["data"][index]["Subcategory"] = t->sub_category;
            root["records"][i]["data"][index]["Type"] = t->type;
            root["records"][i]["data"][index]["AccountFrom"] = t->account_id;
            root["records"][i]["data"][index]["AccountTo"] = t->account_to;
            root["records"][i]["data"][index]["Amount"] = t->amount;
            root["records"][i]["data"][index]["Comment"] = t->comment;
        }
    }

    if (!found) {
        int last_index = root["records"].size();
        root["records"][last_index]["Month"] = month;
        root["records"][last_index]["Year"] = year;

        root["records"][last_index]["data"][0]["Day"] = t->day;
        root["records"][last_index]["data"][0]["Category"] = t->category;
        root["records"][last_index]["data"][0]["Subcategory"] = t->sub_category;
        root["records"][last_index]["data"][0]["Type"] = t->type;
        root["records"][last_index]["data"][0]["AccountFrom"] = t->account_id;
        root["records"][last_index]["data"][0]["AccountTo"] = t->account_to;
        root["records"][last_index]["data"][0]["Amount"] = t->amount;
        root["records"][last_index]["data"][0]["Comment"] = t->comment;
    }

    // transaction sort
    root = BubbleSortTransactions(root);
    // Maybe a month sorting is required ?

    // Write the output to a file
    writeToFileStream(root_directory_db + "/incomeExpenses.json", root);
}

// Integration

void Backend::updateAccountsDetailsData(int month, int year, Transaction_p t)
{
    Json::Value root;
    bool month_found = false;
    int index_record_change = -1;

    root = getRootFromFileStream(root_directory_db + "/accountsDetails.json");

    for (unsigned int i = 0; i < root["records"].size() && !month_found; i++) {
        if ((root["records"][i]["Year"] == year) && (root["records"][i]["Month"] == month)) {
            month_found = true;
            index_record_change = i;
            bool accountId_found = false;
            bool accountTo_found = false;
            unsigned int size_data = root["records"][i]["data"].size();
            for (int j = 0; j < size_data; j++) {
                if(t->account_to != -1){
                    if (root["records"][i]["data"][j]["id"] == t->account_id) {
                        accountId_found = true;
                        double old_amount = std::stod(root["records"][i]["data"][j]["Amount"].asString());
                        root["records"][i]["data"][j]["Amount"] = old_amount - t->amount;
                    }
                    if (root["records"][i]["data"][j]["id"] == t->account_to) {
                        accountTo_found = true;
                        double old_amount = std::stod(root["records"][i]["data"][j]["Amount"].asString());
                        root["records"][i]["data"][j]["Amount"] = old_amount + t->amount;
                    }
                }else{
                    if (root["records"][i]["data"][j]["id"] == t->account_id) {
                        accountId_found = true;
                        double old_amount = std::stod(root["records"][i]["data"][j]["Amount"].asString());
                        root["records"][i]["data"][j]["Amount"] = old_amount + t->amount;
                    }
                }
            }

            if (!accountId_found) {
                if(t->account_to != -1){
                    root["records"][i]["data"][size_data]["id"] = t->account_id;
                    root["records"][i]["data"][size_data]["Amount"] = getAccountAmountAt(t->account_id, month, year) - t->amount;
                    size_data++;
                }else{
                    root["records"][i]["data"][size_data]["id"] = t->account_id;
                    root["records"][i]["data"][size_data]["Amount"] = getAccountAmountAt(t->account_id, month, year) + t->amount;
                }
            }
            if(!accountTo_found) {
                if(t->account_to != -1){
                    root["records"][i]["data"][size_data]["id"] = t->account_to;
                    root["records"][i]["data"][size_data]["Amount"] = getAccountAmountAt(t->account_to, month, year) + t->amount;
                }
            }
        }
    }

    if (!month_found) {
        int last_index = root["records"].size();
        root["records"][last_index]["Month"] = month;
        root["records"][last_index]["Year"] = year;
        
        if(t->account_to != -1){
            root["records"][last_index]["data"][0]["id"] = t->account_id;
            root["records"][last_index]["data"][0]["Amount"] = getAccountAmountAt(t->account_id, month, year) - t->amount;

            root["records"][last_index]["data"][1]["id"] = t->account_to;
            root["records"][last_index]["data"][1]["Amount"] = getAccountAmountAt(t->account_to, month, year) + t->amount;
        }else{
            root["records"][last_index]["data"][0]["id"] = t->account_id;
            root["records"][last_index]["data"][0]["Amount"] = getAccountAmountAt(t->account_id, month, year) + t->amount;
        }

        // sorting required after new month insertion
        root = BubbleSortAccountDetails(root);
    } 
    else {
        // Propagate changes in the next month records
        for (unsigned int i = index_record_change + 1; i < root["records"].size(); i++) {
            for (unsigned int j = 0; j < root["records"][i]["data"].size(); j++) {
                if(t->account_to != -1){
                    if (root["records"][i]["data"][j]["id"] == t->account_id) {
                        root["records"][i]["data"][j]["Amount"] = std::stod(root["records"][i]["data"][j]["Amount"].asString()) - t->amount;
                    }
                    else if (root["records"][i]["data"][j]["id"] == t->account_to) {
                        root["records"][i]["data"][j]["Amount"] = std::stod(root["records"][i]["data"][j]["Amount"].asString()) + t->amount;
                    }
                }else{
                    if (root["records"][i]["data"][j]["id"] == t->account_id) {
                        root["records"][i]["data"][j]["Amount"] = std::stod(root["records"][i]["data"][j]["Amount"].asString()) + t->amount;
                    }
                }
            }
        }
    }

    // Write the output to a file
    writeToFileStream(root_directory_db + "/accountsDetails.json", root);
}

void Backend::updateNetWorthData(int month, int year, Transaction_p t)
{
    Json::Value root;
    bool monthfound = false;
    int index_record_change = -1;

    root = getRootFromFileStream(root_directory_db + "/netWorth.json");

    // Implementing the opening, low, high, closing calculation based on temporal sequence
    double last_nw = getNWat(month, year);
    double OpeningWorth = last_nw;
    double LowWorth = last_nw;
    double HighWorth = last_nw;
    double ClosingWorth = last_nw;
    std::vector<Transaction_p> transactions = getMonthlyReport(month, year)->transactions;
    for (auto x : transactions) {
        ClosingWorth += x->amount;
        if (x->type == "In") {
            if (ClosingWorth > HighWorth)
                HighWorth = ClosingWorth;
        }
        else if(x->type == "Out") {
            if (ClosingWorth < LowWorth)
                LowWorth = ClosingWorth;
        }
    }

    // Update NW records file
    for (unsigned int i = 0; i < root["records"].size() && !monthfound; i++) {
        if ((root["records"][i]["Year"] == year) && (root["records"][i]["Month"] == month)) {
            monthfound = true;
            index_record_change = i;
            root["records"][i]["ClosingWorth"] = ClosingWorth;
            root["records"][i]["HighWorth"] = HighWorth;
            root["records"][i]["LowWorth"] = LowWorth;
        }
    }

    if (!monthfound) {
        unsigned int last_index = root["records"].size();
        root["records"][last_index]["Month"] = month;
        root["records"][last_index]["Year"] = year;

        root["records"][last_index]["OpeningWorth"] = last_nw;
        root["records"][last_index]["LowWorth"] = LowWorth;
        root["records"][last_index]["HighWorth"] = HighWorth;
        root["records"][last_index]["ClosingWorth"] = ClosingWorth;

        // sorting required after new month insertion
        root = BubbleSortNetWorth(root);
    }
    else {
        // Propagate changes in the next month records
        for (unsigned int i = index_record_change + 1; i < root["records"].size(); i++) {
            root["records"][i]["OpeningWorth"] = std::stod(root["records"][i]["OpeningWorth"].asString()) + t->amount;
            root["records"][i]["LowWorth"] = std::stod(root["records"][i]["LowWorth"].asString()) + t->amount;
            root["records"][i]["HighWorth"] = std::stod(root["records"][i]["HighWorth"].asString()) + t->amount;
            root["records"][i]["ClosingWorth"] = std::stod(root["records"][i]["ClosingWorth"].asString()) + t->amount;
        }
    }

    // Write the output to a file
    writeToFileStream(root_directory_db + "/netWorth.json", root);
}

double Backend::getAccountAmountAt(int id, int month, int year)
{
    double amount = 0.0;
    std::shared_ptr<AccountMonthlyDetails> data = getAccountMonthlyRecords(id);

    int currentMonth_index = -1;
    bool month_found = false;
    for (int i = 0; i < data->accountMonthlyRecords.size() && !month_found; i++) {
        if (data->accountMonthlyRecords[i]->Month == month && data->accountMonthlyRecords[i]->Year == year) {
            month_found = true;
        }
        if ((data->accountMonthlyRecords[i]->Year * 12 + data->accountMonthlyRecords[i]->Month) <= (year * 12 + month)) {
            currentMonth_index = i;
        }
    }

    if (currentMonth_index != -1) {
        amount = data->accountMonthlyRecords[currentMonth_index]->Amount;
    }

    return amount;
}

double Backend::getLastAccountAmount(int id)
{
    double amount = 0.0;
    std::shared_ptr<AccountMonthlyDetails> data = getAccountMonthlyRecords(id);

    if(data->accountMonthlyRecords.size() != 0)
        amount = data->accountMonthlyRecords[data->accountMonthlyRecords.size() - 1]->Amount;

    return amount;
}

double Backend::getLastSubAccountAmount(int acc_id, int sub_acc_id)
{
    return 0.0;
}

double Backend::getNWat(int month, int year)
{
    double amount = 0.0;
    std::vector<NW_record_p> data = getNWdata(-1, -1);

    int currentMonth_index = -1;
    bool month_found = false;
    for (int i = 0; i < data.size() && !month_found; i++) {
        if (data[i]->Month == month && data[i]->Year == year) {
            month_found = true;
        }
        if ((data[i]->Year * 12 + data[i]->Month) < (year * 12 + month)) {
            currentMonth_index = i;
        }
    }

    if (currentMonth_index != -1) {
        amount = data[currentMonth_index]->ClosingWorth;
    }

    return amount;
}

// Aggregate Reports

MonthlyAggrCategoryReport_p Backend::getAggrCatReport(int month, int year)
{
    MonthlyAggrCategoryReport_p x = std::make_shared<MonthlyAggrCategoryReport>();
    x->Month = month;
    x->Year = year;

    std::vector<Category_p> categories = this->getCategories();
    for(const auto& cat : categories){
        if(cat->type == "Out"){
            MonthlyCategoryBalanceT_p balance = std::make_shared<MonthlyCategoryBalanceT>();
            balance->Category = cat->name;
            balance->CategoryType = cat->type;
            balance->Month = month;
            balance->Year = year;
            balance->Amount = this->getAmountByCategory(month, year, cat->name);
            x->totalsByCategory.push_back(balance);
        }
    }

    return x;
}

double Backend::getAmountByCategory(int month, int year, const std::string& category)
{
    double amount = 0.0;
    MonthlyTransactions_p x = std::make_shared<MonthlyTransactions>();
    x = this->getMonthlyReport(month, year);

    for(const auto& t : x->transactions){
        if(t->category == category){
            amount = t->amount;
        }
    }

    return amount;
}

MonthlyAggrCategoryReport_p Backend::getAggrCatReportWithoutInvestments(int month, int year)
{
    MonthlyAggrCategoryReport_p x = std::make_shared<MonthlyAggrCategoryReport>();
    x->Month = month;
    x->Year = year;

    std::vector<Category_p> categories = this->getCategories();
    for(const auto& cat : categories){
        if(cat->type == "Out" && cat->name != "Investments (+)" && cat->name != "Investments (-)"){
            MonthlyCategoryBalanceT_p balance = std::make_shared<MonthlyCategoryBalanceT>();
            balance->Category = cat->name;
            balance->CategoryType = cat->type;
            balance->Month = month;
            balance->Year = year;
            balance->Amount = this->getAmountByCategory(month, year, cat->name);
            x->totalsByCategory.push_back(balance);
        }
    }

    return x;
}

// Investments

YearlyInvestmentsReport_p Backend::getYearlyInvestmentsReport(int year)
{
    YearlyInvestmentsReport_p yearlyReport = std::make_shared<YearlyInvestmentsReport>();
    yearlyReport->Year = year;
    yearlyReport->monthlyInvestmentsReports = std::vector<MonthlyInvestmentsReport_p>();

    for (int i = 0; i < 12; i++) {
        MonthlyTransactions_p monthlyReport = this->getMonthlyReport(i + 1, year);
        MonthlyInvestmentsReport_p x = std::make_shared<MonthlyInvestmentsReport>();
        x->Month = i + 1;
        x->Year = year;
        x->initial_capital = getAccountAmountAt(1, i + 1, year);
        for (const auto& t : monthlyReport->transactions) {
            if (t->category == "Investments (+)") {
                if (t->sub_category == "Investments (+)") x->investments_variation += t->amount;
            } 
            else if (t->category == "Investments (-)") {
                if (t->sub_category == "Investments (-)") x->investments_variation += t->amount;
            }
            else if (t->category == "Transfer") {
                if (t->sub_category == "Deposit") x->deposits += t->amount;
                if (t->sub_category == "Withdrawal") x->deposits -= t->amount;
            }
        }

        yearlyReport->monthlyInvestmentsReports.push_back(x);
    }

    return yearlyReport;
}

AllTimeInvestmentsReport_p Backend::getAllTimeInvestmentsReport()
{
    Json::Value root;

    AllTimeInvestmentsReport_p res = std::make_shared<AllTimeInvestmentsReport>();

    int first_investment_year = -1;
    int last_investment_year = -1;
    getFirstAndLastInvestmentYears(first_investment_year, last_investment_year);

    if( first_investment_year == -1 || last_investment_year == -1) {
        // Something went wrong, just show current year data
        res->yearlyInvestmentsReports.push_back(getYearlyInvestmentsReport(getCurrentYear()));
    } else {
        for (int i = first_investment_year; i < last_investment_year + 1; i++) {
            res->yearlyInvestmentsReports.push_back(getYearlyInvestmentsReport(i));
        }
    }

    return res;
}

Config_p Backend::getConfig()
{
    Json::Value root;
    Config_p config = std::make_shared<Config_t>();
    
    root = getRootFromFileStream(root_fonts + "/fonts/config.json");

    config->display_resolution = root["display_resolution"].asString();
    config->default_font = root["default_font"].asString();

    return config;
}

// Private

Json::Value Backend::getRootFromFileStream(const std::string& ifstream_name)
{
    Json::Reader reader;
    Json::Value root;
    std::ifstream acc_file;

    acc_file.open(ifstream_name);
    if (acc_file.is_open()) {
        // Parse JSON and print errors if needed
        if (!reader.parse(acc_file, root)) {
            std::cout << reader.getFormattedErrorMessages();
            exit(1);
        }
        else {
            return root;
        }
    }

    return root;
}

void Backend::writeToFileStream(const std::string& ofstream_name, const Json::Value& root)
{
    std::ofstream outFile;
    Json::StyledStreamWriter writer;

    outFile.open(ofstream_name);
    writer.write(outFile, root);
    outFile.close();
}

void Backend::getFirstAndLastInvestmentYears(int& first, int& last)
{
    last = getCurrentYear();
    for(int i=getCurrentYear(); i>MINIMUM_INVESTMENT_YEAR; i--) {
        YearlyInvestmentsReport_p x = getYearlyInvestmentsReport(i);
        bool some_activity = false;
        for(auto elem: x->monthlyInvestmentsReports) {
            if(elem->deposits != 0 || elem->investments_variation != 0) {
                some_activity = true;
            }
        }
        if(some_activity) {
            first = i;
        }
    }
}
