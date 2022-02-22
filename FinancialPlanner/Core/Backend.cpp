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
    root["accounts"][nextIndex]["AmountStored"] = x->AmountStored;

    // Write the output to a file
    writeToFileStream("Database/accounts.json", root);

    return;
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

        NW_data.push_back(x);
    }

    return NW_data;
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
