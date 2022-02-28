#pragma once

#include <iostream>
#include <string>
#include "Backend.h"
#include "../Utility/Utility.h"

class Core {
	Backend back_end;

	std::vector<Account_p> accounts;
	std::vector<Category_p> categories;

	std::vector<NW_record_p> NW_records;
	MonthlyTransactions_p MonthlyReport;
	YearlyReport_p YearlyReport;
	//MonthlyAggrAccountReport_p monthlyAccountsReport;
	//MonthlyAggrSubCategoryReport_p monthlySubCategoryReport;
	//MonthlyAggrCategoryReport_p monthlyCategoryReport;
public:
	Core();

	// Financial Functions =======
	float CompoundInterestCalculate(float initialNW, float interestRate, float annualDeposits, int investmentYears, float* y_data);
	
	// Backend Functions =========

	// Accounts =====================================
	std::vector<Account_p> getAccounts();
	int getAccountsSize();
	std::vector<Account_p> getAccountsFromDb();
	std::string getAccountName(int id);
	void pushAccount(Account_p x);
	AccountMonthlyDetails_p getAccountMonthlyRecords(int id);
	void deleteAccount(int id);
	int getIDfromIndex(int index);
	// Categories ====================================
	std::vector<Category_p> getCategories();
	std::vector<Category_p> getCategoriesFromDb();
	std::vector<SubCategory_p> getSubCategoriesOf(std::string catName);
	void pushCategory(Category_p x);
	void pushSubCategory(std::string categoryName, SubCategory_p);
	bool checkCategoryExists(std::string name);
	bool checkErrors(std::string cat, std::string subCat, std::string type, double amount, int year);
	void deleteCategory(int id);
	// Net Worth (NW) =====================================
	std::vector<NW_record_p> getNWdata();
	std::vector<NW_record_p> getNWdataFromDb(double from, double to);
	// Income / Expenses =====================================
	MonthlyTransactions_p getMonthlyTransactionsReport();
	MonthlyTransactions_p getMonthlyTransactionsReportFromDb(int month, int year);
	YearlyReport_p getYearlyReport();
	YearlyReport_p getYearlyReportFromDb(int year);
	void pushTransaction(int month, int year, Transaction_p t);

	// Testing
	std::string testBackend();
};