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

// Accounts detail Monthly record

struct AccountMonthlyRecord {
	int AccountID;
	int Amount;
};
using AccountMonthlyRecord_p = AccountMonthlyRecord*;

struct AccountsMonthlyDetail {
	int Month;
	int Year;
	std::vector<AccountMonthlyRecord_p> accountMonthlyRecords;
};
using AccountsMonthlyDetail_p = AccountsMonthlyDetail*;

// Account Detail Monthly Records
struct AccountMonthlyRecordComplex {
	int Month;
	int Year;
	int Amount;
};
using AccountMonthlyRecordComplex_p = AccountMonthlyRecordComplex*;

struct AccountMonthlyDetails {
	int AccountID;
	std::vector<AccountMonthlyRecordComplex_p> accountMonthlyRecords;
};
using AccountMonthlyDetails_p = AccountMonthlyDetails*;

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

// Subcategory
struct SubCategory {
	int id;
	std::string Name;
};
using SubCategory_p = SubCategory*;

// Category
struct Category {
	int id;
	std::string Name;
	std::string Type;
	std::vector<SubCategory_p> subCategories;
};
using Category_p = Category*;

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

struct MonthlyCategoryBalanceT {
	std::string Category;
	std::string CategoryType;
	int Month;
	int Year;
	double Amount;
};
using MonthlyCategoryBalanceT_p = MonthlyCategoryBalanceT*;

struct MonthlySubCategoryBalanceT {
	std::string SubCategory;
	std::string Category;
	std::string CategoryType;
	int Month;
	int Year;
	double Amount;
};
using MonthlySubCategoryBalanceT_p = MonthlySubCategoryBalanceT*;

struct MonthlyAccountBalanceT {
	int AccountID;
	int Month;
	int Year;
	double balanceIn;
	double balanceOut;
};
using MonthlyAccountBalanceT_p = MonthlyAccountBalanceT*;

struct MonthlyAggrAccountReport {
	int Month;
	int Year;
	std::vector<MonthlyAccountBalanceT_p> totalsByAccountID;
};
using MonthlyAggrAccountReport_p = MonthlyAggrAccountReport*;

struct MonthlyAggrSubCategoryReport {
	int Month;
	int Year;
	std::vector<MonthlySubCategoryBalanceT_p> totalsBySubCategory;
};
using MonthlyAggrSubCategoryReport_p = MonthlyAggrSubCategoryReport;

struct MonthlyAggrCategoryReport {
	int Month;
	int Year;
	std::vector<MonthlyCategoryBalanceT_p> totalsByCategory;
};
using MonthlyAggrCategoryReport_p = MonthlyAggrCategoryReport;

struct MonthlyReport {
	int Month;
	int Year;
	double balanceIn;
	double balanceOut;
};
using MonthlyReport_p = MonthlyReport*;

struct YearlyReport {
	int Year;
	std::vector<MonthlyReport_p> monthlyReports;
};
using YearlyReport_p = YearlyReport*;

class Backend {
public:
	void init();

	// Accounts
	std::vector<Account_p> getAccounts();
	void pushAccount(Account_p x);
	AccountMonthlyDetails_p getAccountMonthlyRecords(int id);

	// Category
	std::vector<Category_p> getCategories();
	void pushCategory(Category_p x);
	void pushSubCategory(std::string categoryName, SubCategory_p x);
	Json::Value SwapLastElements(Json::Value root, int i);

	// Net Worth
	std::vector<NW_record_p> getNWdata(double from, double to);

	// Income / Expenses
	MonthlyTransactions_p getMonthlyReport(int month, int year);
	YearlyReport_p getYearlyReport(int year);
	void pushTransaction(int month, int year, Transaction_p t);

	// Integration
	void updateAccountsDetailsData(int month, int year, Transaction_p t);

	// Testing
	std::string sayHello();

private:
	Json::Value getRootFromFileStream(std::string ifstream_name);
	void writeToFileStream(std::string ofstream_name, Json::Value root);
};