#include "Backend.h"

void Backend::init() {

}

std::vector<Account_p> Backend::getAccounts()
{
    Json::Value root;

    std::vector<Account_p> accounts;

    root = getRootFromFileStream("Database/accounts.json");
    
    Json::Value acc = root["accounts"];

    for (unsigned int i = 0; i < acc.size(); i++) {
        Account_p x = std::make_shared<Account>();
        x->id = std::stoi(acc[i]["id"].asString());
        x->name = acc[i]["name"].asString();
        x->AmountStored = 0.0;
        // calculate Amount stored by each account based on last report
        x->AmountStored = this->getLastAccountAmount(x->id);

        accounts.push_back(x);
    }

    return accounts;
}

void Backend::pushAccount(Account_p x)
{
    Json::Value root;

    root = getRootFromFileStream("Database/accounts.json");

    // push in accounts list
    int nextIndex = root["accounts"].size();
    root["accounts"][nextIndex]["id"] = x->id;
    root["accounts"][nextIndex]["name"] = x->name;

    // Write the output to a file
    writeToFileStream("Database/accounts.json", root);

    return;
}

AccountMonthlyDetails_p Backend::getAccountMonthlyRecords(int id)
{
    Json::Value root;

    double last_amount = 0.0;

    AccountMonthlyDetails_p x = std::make_shared<AccountMonthlyDetails>();
    x->AccountID = id;

    std::vector<Account_p> accounts;

    root = getRootFromFileStream("Database/accountsDetails.json");

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
                x->accountMonthlyRecords.push_back(item);
            }
        }

        if (!found) {
            // Padding an empty month
            item->Amount = last_amount;
            x->accountMonthlyRecords.push_back(item);
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

    root = getRootFromFileStream("Database/accountsDetails.json");

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

    root = getRootFromFileStream("Database/accounts.json");

    Json::Value accounts = root["accounts"];
    Json::Value newArrayAccounts = Json::arrayValue;

    for (unsigned int i = 0; i < accounts.size(); i++) {
        if (accounts[i]["id"].asInt() != id) {
            // push in new vector
            int newArraySize = newArrayAccounts.size();
            newArrayAccounts[newArraySize]["id"] = accounts[i]["id"];
            newArrayAccounts[newArraySize]["name"] = accounts[i]["name"];
        }
    }
    root["accounts"] = newArrayAccounts;

    // Write the output to a file
    writeToFileStream("Database/accounts.json", root);

    return;

}

std::vector<Category_p> Backend::getCategories()
{
    Json::Value root;

    std::vector<Category_p> categories;

    root = getRootFromFileStream("Database/categories.json");

    Json::Value cat = root["categories"];

    for (unsigned int i = 0; i < cat.size(); i++) {
        Category_p x = std::make_shared<Category>();
        x->id = std::stoi(cat[i]["id"].asString());
        x->Name = cat[i]["Name"].asString();
        x->Type = cat[i]["Type"].asString();
        for (unsigned int j = 0; j < cat[i]["SubCategories"].size(); j++) {
            SubCategory_p s = std::make_shared<SubCategory>();
            s->id = std::stoi(cat[i]["SubCategories"][j]["id"].asString());
            s->Name = cat[i]["SubCategories"][j]["Name"].asString();
            x->subCategories.push_back(s);
        }

        categories.push_back(x);
    }

    return categories;
}

void Backend::pushCategory(Category_p x)
{
    Json::Value root;

    root = getRootFromFileStream("Database/categories.json");

    int nextIndex = root["categories"].size();
    root["categories"][nextIndex]["id"] = x->id;
    root["categories"][nextIndex]["Name"] = x->Name;
    root["categories"][nextIndex]["Type"] = x->Type;
    for (int i = 0; i < x->subCategories.size(); i++) {
        root["categories"][nextIndex]["SubCategories"][i]["id"] = x->subCategories[i]->id;
        root["categories"][nextIndex]["SubCategories"][i]["Name"] = x->subCategories[i]->Name;
    }

    // Write the output to a file
    writeToFileStream("Database/categories.json", root);

    return;
}

void Backend::pushSubCategory(std::string categoryName, SubCategory_p x)
{
    Json::Value root;
    int index = -1;

    root = getRootFromFileStream("Database/categories.json");

    for (unsigned int i = 0; i < root["categories"].size(); i++) {
        if (root["categories"][i]["Name"].asString() == categoryName) {
            index = i;
            int nextIndex = root["categories"][i]["SubCategories"].size();
            root["categories"][i]["SubCategories"][nextIndex]["id"] = nextIndex + 1;
            root["categories"][i]["SubCategories"][nextIndex]["Name"] = x->Name;
        }
    }

    // Other + category name at the end of the list
    root = SwapLastElements(root, index);

    // Write the output to a file
    writeToFileStream("Database/categories.json", root);

    return;
}

Json::Value Backend::SwapLastElements(Json::Value root, int i)
{
    std::string tmp;
    int lastIndex = root["categories"][i]["SubCategories"].size() - 1;

    tmp = root["categories"][i]["SubCategories"][lastIndex]["Name"].asString();
    root["categories"][i]["SubCategories"][lastIndex]["Name"] = root["categories"][i]["SubCategories"][lastIndex - 1]["Name"];
    root["categories"][i]["SubCategories"][lastIndex - 1]["Name"] = tmp;

    return root;
}

void Backend::deleteCategory(int id)
{
    Json::Value root;

    root = getRootFromFileStream("Database/categories.json");

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
    writeToFileStream("Database/categories.json", root);

    return;
}

std::vector<NW_record_p> Backend::getNWdata(double from, double to)
{
    Json::Value root;

    std::vector<NW_record_p> NW_data;

    root = getRootFromFileStream("Database/netWorth.json");

    Json::Value data = root["records"];

    for (unsigned int i = 0; i < data.size(); i++) {
        NW_record_p x = std::make_shared<NW_record>();
        x->Month = std::stoi(data[i]["Month"].asString());
        x->Year = std::stoi(data[i]["Year"].asString());
        x->OpeningWorth = std::stod(data[i]["OpeningWorth"].asString());
        x->LowWorth = std::stod(data[i]["LowWorth"].asString());
        x->HighWorth = std::stod(data[i]["HighWorth"].asString());
        x->ClosingWorth = std::stod(data[i]["ClosingWorth"].asString());

        double UnixTime = getUNIXtime(x->Month, x->Year);

        if (from == -1 && to == -1) {
                NW_data.push_back(x);
        }
        else {
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

    root = getRootFromFileStream("Database/incomeExpenses.json");

    Json::Value data = root["records"];
    Json::Value transactRecords;

    for (unsigned int i = 0; i < data.size(); i++) {
        if (std::stoi(data[i]["Month"].asString()) == month && std::stoi(data[i]["Year"].asString()) == year) {
            MonthlyReport->Month = month;
            MonthlyReport->Year = year;
            transactRecords = data[i]["data"];
            for (unsigned int i = 0; i < transactRecords.size(); i++) {
                Transaction_p t = std::make_shared<Transaction>();
                t->Day = std::stoi(transactRecords[i]["Day"].asString());
                t->Category = transactRecords[i]["Category"].asString();
                t->Subcategory = transactRecords[i]["Subcategory"].asString();
                t->Type = transactRecords[i]["Type"].asString();
                t->AccountID = std::stoi(transactRecords[i]["Account"].asString());
                t->Amount = std::stod(transactRecords[i]["Amount"].asString());
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
        for (auto t : monthlyReport->transactions) {
            if (t->Type == "In")
                x->balanceIn += t->Amount;
            if (t->Type == "Out")
                x->balanceOut += t->Amount;
        }

        yearlyReport->monthlyReports.push_back(x);
    }

    return yearlyReport;
}

void Backend::pushTransaction(int month, int year, Transaction_p t)
{
    Json::Value root;
    bool found = false;

    root = getRootFromFileStream("Database/incomeExpenses.json");

    for (unsigned int i = 0; i < root["records"].size(); i++) {
        if ((root["records"][i]["Year"] == year) && (root["records"][i]["Month"] == month)) {
            found = true;
            int index = root["records"][i]["data"].size();
            root["records"][i]["data"][index]["Day"] = t->Day;
            root["records"][i]["data"][index]["Category"] = t->Category;
            root["records"][i]["data"][index]["Subcategory"] = t->Subcategory;
            root["records"][i]["data"][index]["Type"] = t->Type;
            root["records"][i]["data"][index]["Account"] = t->AccountID;
            root["records"][i]["data"][index]["Amount"] = t->Amount;
        }
    }

    if (!found) {
        int last_index = root["records"].size();
        root["records"][last_index]["Month"] = month;
        root["records"][last_index]["Year"] = year;

        root["records"][last_index]["data"][0]["Day"] = t->Day;
        root["records"][last_index]["data"][0]["Category"] = t->Category;
        root["records"][last_index]["data"][0]["Subcategory"] = t->Subcategory;
        root["records"][last_index]["data"][0]["Type"] = t->Type;
        root["records"][last_index]["data"][0]["Account"] = t->AccountID;
        root["records"][last_index]["data"][0]["Amount"] = t->Amount;
    }

    // transaction sort
    root = BubbleSortTransactions(root);
    // Maybe a month sorting is required ?

    // Write the output to a file
    writeToFileStream("Database/incomeExpenses.json", root);

    return;
}

// Integration

void Backend::updateAccountsDetailsData(int month, int year, Transaction_p t)
{
    Json::Value root;
    bool monthfound = false;
    int index_record_change = -1;

    root = getRootFromFileStream("Database/accountsDetails.json");

    for (unsigned int i = 0; i < root["records"].size() && !monthfound; i++) {
        if ((root["records"][i]["Year"] == year) && (root["records"][i]["Month"] == month)) {
            monthfound = true;
            index_record_change = i;
            bool accountId_found = false;
            int size_data = root["records"][i]["data"].size();
            for (int j = 0; j < size_data; j++) {
                if (root["records"][i]["data"][j]["id"] == t->AccountID) {
                    accountId_found = true;
                    double old_amount = std::stod(root["records"][i]["data"][j]["Amount"].asString());
                    root["records"][i]["data"][j]["Amount"] = old_amount + t->Amount;
                }
            }

            if (!accountId_found) {
                root["records"][i]["data"][size_data]["id"] = t->AccountID;
                root["records"][i]["data"][size_data]["Amount"] = getAccountAmountAt(t->AccountID, month, year) + t->Amount;
            }
        }
    }

    if (!monthfound) {
        int last_index = root["records"].size();
        root["records"][last_index]["Month"] = month;
        root["records"][last_index]["Year"] = year;
        
        root["records"][last_index]["data"][0]["id"] = t->AccountID;
        root["records"][last_index]["data"][0]["Amount"] = getAccountAmountAt(t->AccountID, month, year) + t->Amount;

        // sorting required after new month insertion
        root = BubbleSortAccountDetails(root);
    } 
    else {
        // Propagate changes in the next month records
        for (unsigned int i = index_record_change + 1; i < root["records"].size(); i++) {
            for (unsigned int j = 0; j < root["records"][i]["data"].size(); j++) {
                if (root["records"][i]["data"][j]["id"] == t->AccountID) {
                    root["records"][i]["data"][j]["Amount"] = std::stod(root["records"][i]["data"][j]["Amount"].asString()) + t->Amount;
                }
            }
        }
    }

    // Write the output to a file
    writeToFileStream("Database/accountsDetails.json", root);
}

void Backend::updateNetWorthData(int month, int year, Transaction_p t)
{
    Json::Value root;
    bool monthfound = false;
    int index_record_change = -1;

    root = getRootFromFileStream("Database/netWorth.json");

    // Implementing the opening, low, high, closing calculation based on temporal sequence
    double last_nw = getNWat(month, year);
    double OpeningWorth = last_nw;
    double LowWorth = last_nw;
    double HighWorth = last_nw;
    double ClosingWorth = last_nw;
    std::vector<Transaction_p> transactions = getMonthlyReport(month, year)->transactions;
    for (auto x : transactions) {
        ClosingWorth += x->Amount;
        if (x->Type == "In") {
            if (ClosingWorth > HighWorth)
                HighWorth = ClosingWorth;
        }
        else if(x->Type == "Out") {
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
        int last_index = root["records"].size();
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
            root["records"][i]["OpeningWorth"] = std::stod(root["records"][i]["OpeningWorth"].asString()) + t->Amount;
            root["records"][i]["LowWorth"] = std::stod(root["records"][i]["LowWorth"].asString()) + t->Amount;
            root["records"][i]["HighWorth"] = std::stod(root["records"][i]["HighWorth"].asString()) + t->Amount;
            root["records"][i]["ClosingWorth"] = std::stod(root["records"][i]["ClosingWorth"].asString()) + t->Amount;
        }
    }

    // Write the output to a file
    writeToFileStream("Database/netWorth.json", root);
}

double Backend::getAccountAmountAt(int id, int month, int year)
{
    double amount = 0.0;
    std::shared_ptr<AccountMonthlyDetails> data = getAccountMonthlyRecords(id);

    int currentMonth_index = -1;
    bool Monthfound = false;
    for (int i = 0; i < data->accountMonthlyRecords.size() && !Monthfound; i++) {
        if (data->accountMonthlyRecords[i]->Month == month && data->accountMonthlyRecords[i]->Year == year) {
            Monthfound = true;
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

double Backend::getNWat(int month, int year)
{
    double amount = 0.0;
    std::vector<NW_record_p> data = getNWdata(-1, -1);

    int currentMonth_index = -1;
    bool Monthfound = false;
    for (int i = 0; i < data.size() && !Monthfound; i++) {
        if (data[i]->Month == month && data[i]->Year == year) {
            Monthfound = true;
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
    for(auto cat : categories){
        if(cat->Type == "Out"){
            MonthlyCategoryBalanceT_p balance = std::make_shared<MonthlyCategoryBalanceT>();
            balance->Category = cat->Name;
            balance->CategoryType = cat->Type;
            balance->Month = month;
            balance->Year = year;
            balance->Amount = this->getAmountByCategory(month, year, cat->Name);
            x->totalsByCategory.push_back(balance);
        }
    }

    return x;
}

double Backend::getAmountByCategory(int month, int year, std::string category)
{
    double amount = 0.0;
    MonthlyTransactions_p x = std::make_shared<MonthlyTransactions>();
    x = this->getMonthlyReport(month, year);

    for(auto t : x->transactions){
        if(t->Category == category){
            amount = t->Amount;
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
    for(auto cat : categories){
        if(cat->Type == "Out" && cat->Name != "Investments (+)" && cat->Name != "Investments (-)"){
            MonthlyCategoryBalanceT_p balance = std::make_shared<MonthlyCategoryBalanceT>();
            balance->Category = cat->Name;
            balance->CategoryType = cat->Type;
            balance->Month = month;
            balance->Year = year;
            balance->Amount = this->getAmountByCategory(month, year, cat->Name);
            x->totalsByCategory.push_back(balance);
        }
    }

    return x;
}

// Testing

std::string Backend::sayHello()
{
    Json::Reader reader;
    Json::Value root;
    Json::StyledStreamWriter writer;
    std::string text = "{ \"first\": \"James\", \"last\": \"Bond\", \"nums\": [0, 0, 7] }";
    std::ofstream outFile;

    
    // Parse JSON and print errors if needed
    if (!reader.parse(text, root)) {
        std::cout << reader.getFormattedErrorMessages();
        return "Error : reader.parse(text, root) ";
        exit(1);
    }
    else {

        // Read and modify the json data
        std::cout << "Size: " << root.size() << std::endl;
        std::cout << "Contains nums? " << root.isMember("nums") << std::endl;
        root["first"] = "Jimmy";
        root["middle"] = "Danger";

        // Write the output to a file
        outFile.open("Database/test.json");
        writer.write(outFile, root);
        outFile.close();

        return "Test Completed Successfully";
    }
}

// Private

Json::Value Backend::getRootFromFileStream(std::string ifstream_name)
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
}

void Backend::writeToFileStream(std::string ofstream_name, Json::Value root)
{
    std::ofstream outFile;
    Json::StyledStreamWriter writer;

    outFile.open(ofstream_name);
    writer.write(outFile, root);
    outFile.close();
}
