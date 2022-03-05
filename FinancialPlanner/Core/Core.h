#pragma once

#include <iostream>
#include <string>
#include "Backend.h"
#include "../Utility/Utility.h"

/**
 * @brief Core component : Link with financial procedures and backend link
 * 
 */
class Core {
	/**
	 * @brief Backend Database link
	 * 
	 */
	Backend back_end;

	/**
	 * @brief Data Structure to store accounts
	 * 
	 */
	std::vector<Account_p> accounts;

	/**
	 * @brief Data Structure to store categories
	 * 
	 */
	std::vector<Category_p> categories;

	/**
	 * @brief Data Structure to NW-records
	 * 
	 */
	std::vector<NW_record_p> NW_records;

	/**
	 * @brief Data Structure to store monthly report
	 * 
	 */
	MonthlyTransactions_p MonthlyReport;

	/**
	 * @brief Data Structure to store yearly report
	 * 
	 */
	YearlyReport_p YearlyReport;

	//MonthlyAggrAccountReport_p monthlyAccountsReport;
	//MonthlyAggrSubCategoryReport_p monthlySubCategoryReport;
	//MonthlyAggrCategoryReport_p monthlyCategoryReport;
public:
	/**
	 * @brief Construct a new Core object
	 * 
	*/
	Core();

	/**
	 * @brief TODO
	 * 
	 * @param initialNW 
	 * @param interestRate 
	 * @param annualDeposits 
	 * @param investmentYears 
	 * @param y_data 
	 * @return double 
	 */
	double CompoundInterestCalculate(double initialNW, double interestRate, double annualDeposits, int investmentYears, double* y_data);
	
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