#include "Backend.h"

void Backend::init() {

}

std::vector<Account_p> Backend::getAccounts()
{
    Json::Reader reader;
    Json::Value root;
    std::ifstream acc_file;

    std::vector<Account_p> accounts;

    acc_file.open("Database/accounts.json");
    if (acc_file.is_open()) {
        // Parse JSON and print errors if needed
        if (!reader.parse(acc_file, root)) {
            std::cout << reader.getFormattedErrorMessages();
            exit(1);
        }
        else {
            Json::Value acc = root["accounts"];

            for (int i = 0; i < acc.size(); i++) {
                Account_p x = new Account();
                x->id = std::stoi(acc[i]["id"].asString());
                x->name = acc[i]["name"].asString();
                x->AmountStored = std::stof(acc[i]["AmountStored"].asString());

                accounts.push_back(x);
            }

            return accounts;
        }
    }

    acc_file.close();
    return accounts;
}

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
