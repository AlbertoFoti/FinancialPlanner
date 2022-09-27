#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include "jsoncpp/json/json.h"

// Sub Accounts
struct SubAccount {
    int id;
    std::string name;
    double AmountStored;
};
using SubAccount_p = std::shared_ptr<SubAccount>;

// Account
struct Account {
	int id;
	std::string name;
    std::vector<SubAccount_p> sub_accounts;
	double totAmountStored;
};
using Account_p = std::shared_ptr<Account>;

// Accounts detail Monthly record

struct AccountMonthlyRecord {
	int AccountID;
	int Amount;
};
using AccountMonthlyRecord_p = std::shared_ptr<AccountMonthlyRecord>;

struct AccountsMonthlyDetail {
	int Month;
	int Year;
	std::vector<AccountMonthlyRecord_p> accountMonthlyRecords;
};
using AccountsMonthlyDetail_p = std::shared_ptr<AccountsMonthlyDetail>;

// Account Details Monthly Records
struct AccountMonthlyRecordComplex {
	int Month;
	int Year;
	double Amount;
};
using AccountMonthlyRecordComplex_p = std::shared_ptr<AccountMonthlyRecordComplex>;

struct AccountMonthlyDetails {
	int AccountID;
	std::vector<AccountMonthlyRecordComplex_p> accountMonthlyRecords;
};
using AccountMonthlyDetails_p = std::shared_ptr<AccountMonthlyDetails>;

// NW record
struct NW_record {
	int Month;
	int Year;
	double OpeningWorth;
	double LowWorth;
	double HighWorth;
	double ClosingWorth;
};
using NW_record_p = std::shared_ptr<NW_record>;

// Subcategory
struct SubCategory {
	int id;
	std::string name;
};
using SubCategory_p = std::shared_ptr<SubCategory>;

// Category
struct Category {
	int id;
	std::string name;
	std::string type;
	std::vector<SubCategory_p> subCategories;
};
using Category_p = std::shared_ptr<Category>;

// Transaction
struct Transaction {
	int day;
	std::string category;
	std::string sub_category;
	std::string type;
	int account_id;
	int account_to;
	double amount;
	std::string comment;
};
using Transaction_p = std::shared_ptr<Transaction>;

// Monthly Transactions Summary
struct MonthlyTransactions {
	int Month;
	int Year;
	std::vector<Transaction_p> transactions;
};
using MonthlyTransactions_p = std::shared_ptr<MonthlyTransactions>;

// Monthly Category Balance
struct MonthlyCategoryBalanceT {
	std::string Category;
	std::string CategoryType;
	int Month;
	int Year;
	double Amount;
};
using MonthlyCategoryBalanceT_p = std::shared_ptr<MonthlyCategoryBalanceT>;

// Monthly Subcategory Balance
struct MonthlySubCategoryBalanceT {
	std::string SubCategory;
	std::string Category;
	std::string CategoryType;
	int Month;
	int Year;
	double Amount;
};
using MonthlySubCategoryBalanceT_p = std::shared_ptr<MonthlySubCategoryBalanceT>;

struct MonthlyAccountBalanceT {
	int AccountID;
	int Month;
	int Year;
	double balanceIn;
	double balanceOut;
};
using MonthlyAccountBalanceT_p = std::shared_ptr<MonthlyAccountBalanceT>;

struct MonthlyAggrAccountReport {
	int Month;
	int Year;
	std::vector<MonthlyAccountBalanceT_p> totalsByAccountID;
};
using MonthlyAggrAccountReport_p = std::shared_ptr<MonthlyAggrAccountReport>;

// Aggregate Subcategory balance
struct MonthlyAggrSubCategoryReport {
	int Month;
	int Year;
	std::vector<MonthlySubCategoryBalanceT_p> totalsBySubCategory;
};
using MonthlyAggrSubCategoryReport_p = std::shared_ptr<MonthlyAggrSubCategoryReport>;

// Aggregate Category balance
struct MonthlyAggrCategoryReport {
	int Month;
	int Year;
	std::vector<MonthlyCategoryBalanceT_p> totalsByCategory;
};
using MonthlyAggrCategoryReport_p = std::shared_ptr<MonthlyAggrCategoryReport>;

// Reports
struct MonthlyReport {
	int Month;
	int Year;
	double balanceIn;
	double balanceOut;
	double investmentsVariation;
};
using MonthlyReport_p = std::shared_ptr<MonthlyReport>;

struct YearlyReport {
	int Year;
	std::vector<MonthlyReport_p> monthlyReports;
};
using YearlyReport_p = std::shared_ptr<YearlyReport>;

// Investments Reports
struct MonthlyInvestmentsReport {
	int Month;
	int Year;
	double initial_capital;
	double deposits;
	double investments_variation;
};
using MonthlyInvestmentsReport_p = std::shared_ptr<MonthlyInvestmentsReport>;

struct YearlyInvestmentsReport {
	int Year;
	std::vector<MonthlyInvestmentsReport_p> monthlyInvestmentsReports;
};
using YearlyInvestmentsReport_p = std::shared_ptr<YearlyInvestmentsReport>;

struct AllTimeInvestmentsReport {
    std::vector<YearlyInvestmentsReport_p> yearlyInvestmentsReports;
};
using AllTimeInvestmentsReport_p = std::shared_ptr<AllTimeInvestmentsReport>;

struct Config_t {
	std::string display_resolution;
	std::string default_font;
};
using Config_p = std::shared_ptr<Config_t>;

class Backend {
private:
    static std::string root_directory_db;
    static std::string root_fonts;
public:
	void init();

	/**
	 * @brief Accounts
	 * 
	 */
	std::vector<Account_p> getAccounts();
	void pushAccount(const Account_p& x);
	AccountMonthlyDetails_p getAccountMonthlyRecords(int id);
	AccountMonthlyDetails_p getAccountMonthlyRecordsComplete(int id);
	void deleteAccount(int id);

	/**
	 * @brief Category
	 * 
	 */
	std::vector<Category_p> getCategories();
	void pushCategory(Category_p x);
	void pushSubCategory(const std::string& categoryName, SubCategory_p x);
	static Json::Value SwapLastElements(Json::Value root, int i);
	void deleteCategory(int id);

	/**
	 * @brief Net Worth
	 *  
	 */
	static std::vector<NW_record_p> getNWdata(double from, double to);

	/**
	 * @brief Income / Expenses
	 * 
	 */
	static MonthlyTransactions_p getMonthlyReport(int month, int year);
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
    double getLastSubAccountAmount(int acc_id, int sub_acc_id);
	static double getNWat(int month, int year);

	/**
	 * @brief Aggregate Reports
	 * 
	 */
	MonthlyAggrCategoryReport_p getAggrCatReport(int month, int year);
	double getAmountByCategory(int month, int year, const std::string& category);
	MonthlyAggrCategoryReport_p getAggrCatReportWithoutInvestments(int month, int year);

	/**
	 * @brief Investments Reports
	 * 
	*/
	YearlyInvestmentsReport_p getYearlyInvestmentsReport(int year);
    AllTimeInvestmentsReport_p getAllTimeInvestmentsReport();

	/**
	 * @brief Returns Config Object
	 * 
	 * @return config_t 
	 */
    static Config_p getConfig();

private:
	/**
	 * @brief Private Utility Functions
	 * 
	 */
	static Json::Value getRootFromFileStream(const std::string& ifstream_name);
	static void writeToFileStream(const std::string& ofstream_name, const Json::Value& root);
    void getFirstAndLastInvestmentYears(int& first, int& last);
};