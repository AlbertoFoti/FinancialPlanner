#include "jsoncpp/json/json.h"
#include <string>
#include <fstream>
#include <iostream>

// Testing

std::string sayHello()
{
    Json::Reader reader;
    Json::Value root;
    Json::StyledStreamWriter writer;
    std::string text = R"({ "first": "James", "last": "Bond", "nums": [0, 0, 7] })";
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