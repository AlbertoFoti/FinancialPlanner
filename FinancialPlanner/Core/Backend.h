#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include "json/json.h"
#include "../Utility/Utility.h"

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

// Transaction
struct Transaction {
	int Day;
	std::string Category;
	std::string Subcategory;
	std::string Type;
	int AccountID;
	double Amount;
};
using Transaction_p = Transaction*;

// Monthly Transactions Summary
struct MonthlyTransactions {
	int Month;
	int Year;
	std::vector<Transaction_p> transactions;
};
using MonthlyTransactions_p = MonthlyTransactions*;

class Backend {
public:
	void init();

	// Accounts
	std::vector<Account_p> getAccounts();
	void pushAccount(Account_p x);

	// Net Worth
	std::vector<NW_record_p> getNWdata(double from, double to);

	// Income / Expenses
	MonthlyTransactions_p getMonthlyReport(int month, int year);

	// Testing
	std::string sayHello();

private:
	Json::Value getRootFromFileStream(std::string ifstream_name);
	void writeToFileStream(std::string ofstream_name, Json::Value root);
};