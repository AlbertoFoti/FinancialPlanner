#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include "json/json.h"

// Account
struct Account {
	int id;
	std::string name;
	double AmountStored;
};
using Account_p = Account*;

// NW record
struct NW_record {
	int Month;
	int Year;
	double OpeningWorth;
	double LowWorth;
	double HighWorth;
	double ClosingWorth;
};
using NW_record_p = NW_record*;

class Backend {
public:
	void init();

	// Accounts
	std::vector<Account_p> getAccounts();
	void pushAccount(Account_p x);

	// Net Worth
	std::vector<NW_record_p> getNWdata(double from, double to);

	// Testing
	std::string sayHello();

private:
	Json::Value getRootFromFileStream(std::string ifstream_name);
	void writeToFileStream(std::string ofstream_name, Json::Value root);
};