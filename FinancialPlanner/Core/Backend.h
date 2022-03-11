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

// Account Details Monthly Records
struct AccountMonthlyRecordComplex {
	int Month;
	int Year;
	double Amount;
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

// Monthly Category Balance
struct MonthlyCategoryBalanceT {
	std::string Category;
	std::string CategoryType;
	int Month;
	int Year;
	double Amount;
};
using MonthlyCategoryBalanceT_p = MonthlyCategoryBalanceT*;

// Monthly Subcategory Balance
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

// Aggregate Subcategory balance
struct MonthlyAggrSubCategoryReport {
	int Month;
	int Year;
	std::vector<MonthlySubCategoryBalanceT_p> totalsBySubCategory;
};
using MonthlyAggrSubCategoryReport_p = MonthlyAggrSubCategoryReport*;

// Aggregate Category balance
struct MonthlyAggrCategoryReport {
	int Month;
	int Year;
	std::vector<MonthlyCategoryBalanceT_p> totalsByCategory;
};
using MonthlyAggrCategoryReport_p = MonthlyAggrCategoryReport*;

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

	/**
	 * @brief Accounts
	 * 
	 */
	std::vector<Account_p> getAccounts();
	void pushAccount(Account_p x);
	AccountMonthlyDetails_p getAccountMonthlyRecords(int id);
	AccountMonthlyDetails_p getAccountMonthlyRecordsComplete(int id);
	void deleteAccount(int id);

	/**
	 * @brief Category
	 * 
	 */
	std::vector<Category_p> getCategories();
	void pushCategory(Category_p x);
	void pushSubCategory(std::string categoryName, SubCategory_p x);
	Json::Value SwapLastElements(Json::Value root, int i);
	void deleteCategory(int id);

	/**
	 * @brief Net Worth
	 *  
	 */
	std::vector<NW_record_p> getNWdata(double from, double to);

	/**
	 * @brief Income / Expenses
	 * 
	 */
	MonthlyTransactions_p getMonthlyReport(int month, int year);
	YearlyReport_p getYearlyReport(int year);
	void pushTransaction(int month, int year, Transaction_p t);

	/**
	 * @brief Integration
	 *  
	 */
	void updateAccountsDetailsData(int month, int year, Transaction_p t);
	void updateNetWorthData(int month, int year, Transaction_p t);
	double getAccountAmountAt(int id, int month, int year);
	double getLastAccountAmount(int id);
	double getNWat(int month, int year);

	/**
	 * @brief Aggregate Reports
	 * 
	 */
	MonthlyAggrCategoryReport_p getAggrCatReport(int month, int year);
	double getAmountByCategory(int month, int year, std::string category);
	MonthlyAggrCategoryReport_p getAggrCatReportWithoutInvestments(int month, int year);

	/**
	 * @brief Testing
	 * 
	 */
	std::string sayHello();

private:
	/**
	 * @brief Private Utility Functions
	 * 
	 */
	Json::Value getRootFromFileStream(std::string ifstream_name);
	void writeToFileStream(std::string ofstream_name, Json::Value root);
};