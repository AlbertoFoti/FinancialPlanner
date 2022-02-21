#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include "json/json.h"

struct Account {
	int id;
	std::string name;
	float AmountStored;
};
using Account_p = Account*;

class Backend {
public:
	void init();

	// Accounts
	std::vector<Account_p> getAccounts();
	void pushAccount(Account_p x);

	// Testing
	std::string sayHello();

private:
	Json::Value getRootFromFileStream(std::string ifstream_name);
	void writeToFileStream(std::string ofstream_name, Json::Value root);
};