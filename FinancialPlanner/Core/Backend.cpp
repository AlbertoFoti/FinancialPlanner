#include "Backend.h"
#include "Backend.h"
#include "Backend.h"
#include "Backend.h"
#include "Backend.h"
#include "Backend.h"
#include "Backend.h"
#include "Backend.h"
#include "Backend.h"

void Backend::init() {

}

std::vector<Account_p> Backend::getAccounts()
{
    Json::Value root;

    std::vector<Account_p> accounts;

    root = getRootFromFileStream("Database/accounts.json");

    Json::Value acc = root["accounts"];

    for (int i = 0; i < acc.size(); i++) {
        Account_p x = new Account();
        x->id = std::stoi(acc[i]["id"].asString());
        x->name = acc[i]["name"].asString();
        x->AmountStored = std::stod(acc[i]["AmountStored"].asString());

        accounts.push_back(x);
    }

    return accounts;
}

void Backend::pushAccount(Account_p x)
{
    Json::Value root;

    root = getRootFromFileStream("Database/accounts.json");

    int nextIndex = root["accounts"].size();
    root["accounts"][nextIndex]["id"] = x->id;
    root["accounts"][nextIndex]["name"] = x->name;
    root["accounts"][nextIndex]["InitialAmountStored"] = x->AmountStored;

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

    for (int i = 0; i < cat.size(); i++) {
        Category_p x = new Category();
        x->id = std::stoi(cat[i]["id"].asString());
        x->Name = cat[i]["Name"].asString();
        x->Type = cat[i]["Type"].asString();
        for (int j = 0; j < cat[i]["SubCategories"].size(); j++) {
            SubCategory_p s = new SubCategory();
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

    for (int i = 0; i < root["categories"].size(); i++) {
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

std::vector<NW_record_p> Backend::getNWdata(double from, double to)
{
    Json::Value root;

    std::vector<NW_record_p> NW_data;

    root = getRootFromFileStream("Database/netWorth.json");

    Json::Value data = root["records"];

    for (int i = 0; i < data.size(); i++) {
        NW_record_p x = new NW_record();
        x->Month = std::stoi(data[i]["Month"].asString());
        x->Year = std::stoi(data[i]["Year"].asString());
        x->OpeningWorth = std::stod(data[i]["OpeningWorth"].asString());
        x->LowWorth = std::stod(data[i]["LowWorth"].asString());
        x->HighWorth = std::stod(data[i]["HighWorth"].asString());
        x->ClosingWorth = std::stod(data[i]["ClosingWorth"].asString());

        double UnixTime = getUNIXtime(x->Month, x->Year);

        if(UnixTime >= from && UnixTime <= to)
            NW_data.push_back(x);
    }

    return NW_data;
}

MonthlyTransactions_p Backend::getMonthlyReport(int month, int year)
{
    Json::Value root;

    MonthlyTransactions_p MonthlyReport = new MonthlyTransactions();
    MonthlyReport->Month = month;
    MonthlyReport->Year = year;
    MonthlyReport->transactions = std::vector<Transaction_p>();

    root = getRootFromFileStream("Database/incomeExpenses.json");

    Json::Value data = root["records"];
    Json::Value transactRecords;

    for (int i = 0; i < data.size(); i++) {
        if (std::stoi(data[i]["Month"].asString()) == month && std::stoi(data[i]["Year"].asString()) == year) {
            MonthlyReport->Month = month;
            MonthlyReport->Year = year;
            transactRecords = data[i]["data"];
            for (int i = 0; i < transactRecords.size(); i++) {
                Transaction_p t = new Transaction();
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
    YearlyReport_p yearlyReport = new YearlyReport();
    yearlyReport->Year = year;
    yearlyReport->monthlyReports = std::vector<MonthlyReport_p>();

    for (int i = 0; i < 12; i++) {
        MonthlyTransactions_p monthlyReport = this->getMonthlyReport(i + 1, year);
        MonthlyReport_p x = new MonthlyReport();
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

    for (int i = 0; i < root["records"].size(); i++) {
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
    bool found = false;
    std::vector<Account_p> accounts = getAccounts();

    root = getRootFromFileStream("Database/accountDetails.json");

    for (int i = 0; i < root["records"].size(); i++) {
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

        for (int i = 0; i < accounts.size(); i++) {
            root["records"][last_index]["data"][i]["id"] = accounts[i]->id;
            if (t->AccountID == accounts[i]->id) {
                root["records"][last_index]["data"][i]["Amount"] = accounts[i]->AmountStored + t->Amount;
            }
            else {
                root["records"][last_index]["data"][i]["Amount"] = accounts[i]->AmountStored;
            }
        }
    }

    // sorting required (? probably yes)

    // Write the output to a file
    writeToFileStream("Database/incomeExpenses.json", root);
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
