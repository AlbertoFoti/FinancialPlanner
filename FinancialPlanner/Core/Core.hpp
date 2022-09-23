#pragma once

#include <iostream>
#include <string>
#include "Backend.hpp"

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

	/**
	 * @brief Data Structure to store yearly investments report
	 *
	*/
	YearlyInvestmentsReport_p YearlyInvestmentsReport;

    /**
     * Configuration data for Fonts and sizing
     */
    Config_p config;

	//MonthlyAggrAccountReport_p monthlyAccountsReport;
	//MonthlyAggrSubCategoryReport_p monthlySubCategoryReport;
public:
	/**
	 * @brief Construct a new Core object
	 * 
	*/
	Core();

	/**
	 * @brief Compound Interest Calculation function
	 * 
	 * @param initialNW initial Net Worth (capital)
	 * @param interestRate self-explanatory (ex. 7)
	 * @param annualDeposits self-explanatory
	 * @param investmentYears self-explanatory
	 * @param y_data result vector (year by year)
	 * @return double Final Net Worth (capital)
	 */
	double CompoundInterestCalculate(double initialNW, double interestRate, double annualDeposits, int investmentYears, double* y_data);
	
	/**
	 * @brief Get the Accounts object
	 * 
	 * @return std::vector<Account_p> 
	 */
	std::vector<Account_p> getAccounts();

	/**
	 * @brief Get the Accounts Size object
	 * 
	 * @return int 
	 */
	int getAccountsSize();

	/**
	 * @brief Get the Accounts From Db object
	 * 
	 * @return std::vector<Account_p> 
	 */
	std::vector<Account_p> getAccountsFromDb();

	/**
	 * @brief Get the Account Name object
	 * 
	 * @param id self-explanatory
	 * @return std::string 
	 */
	std::string getAccountName(int id);

	/**
	 * @brief Push Account in Database
	 * 
	 * @param x Account
	 */
	void pushAccount(Account_p x);

	/**
	 * @brief Get the Account Monthly Records object
	 * 
	 * @param id self-explanatory
	 * @return AccountMonthlyDetails_p 
	 */
	AccountMonthlyDetails_p getAccountMonthlyRecords(int id);

	/**
	 * @brief Delete Account
	 * 
	 * @param id self-explanatory
	 */
	void deleteAccount(int id);

	/**
	 * @brief ID from Index in Database records
	 * 
	 * @param index self-explanatory
	 * @return int 
	 */
	int getIDfromIndex(int index);
	
	/**
	 * @brief Get the Categories object
	 * 
	 * @return std::vector<Category_p> 
	 */
	std::vector<Category_p> getCategories();

	/**
	 * @brief Get the Categories From Db object
	 * 
	 * @return std::vector<Category_p> 
	 */
	std::vector<Category_p> getCategoriesFromDb();

	/**
	 * @brief Get the Sub Categories Of object
	 * 
	 * @param catName 
	 * @return std::vector<SubCategory_p> 
	 */
	std::vector<SubCategory_p> getSubCategoriesOf(std::string catName);

	/**
	 * @brief Push category in Database
	 * 
	 * @param x Category
	 */
	void pushCategory(Category_p x);

	/**
	 * @brief Push sub-category in Database
	 * 
	 * @param categoryName main category
	 */
	void pushSubCategory(std::string categoryName, SubCategory_p);

	/**
	 * @brief self-explanatory
	 * 
	 * @param name self-explanatory
	 * @return true self-explanatory
	 * @return false self-explanatory
	 */
	bool checkCategoryExists(std::string name);

	/**
	 * @brief self-explanatory
	 * 
	 * @param cat Category
	 * @param subCat Sub Category
	 * @param type self-explanatory
	 * @param amount self-explanatory
	 * @return true error
	 * @return false everything is fine
	 */
	bool checkErrors(int cat, int subCat, std::string type, double amount);

	/**
	 * @brief delete Category
	 * 
	 * @param id self-explanatory
	 */
	void deleteCategory(int id);

	/**
	 * @brief get Net Worth Data
	 * 
	 * @return std::vector<NW_record_p> 
	 */
	std::vector<NW_record_p> getNWdata();

	/**
	 * @brief get Net Worth Data from Db
	 * 
	 * @param from initial Date (UNIX time)
	 * @param to final Date (UNIX time)
	 * @return std::vector<NW_record_p> 
	 */
	std::vector<NW_record_p> getNWdataFromDb(double from, double to);
	
	/**
	 * @brief Get the Monthly Transactions Report object
	 * 
	 * @return MonthlyTransactions_p 
	 */
	MonthlyTransactions_p getMonthlyTransactionsReport();

	/**
	 * @brief Get the Monthly Transactions Report From Db object
	 * 
	 * @param month self-explanatory
	 * @param year self-explanatory
	 * @return MonthlyTransactions_p 
	 */
	MonthlyTransactions_p getMonthlyTransactionsReportFromDb(int month, int year);

	/**
	 * @brief Get the Yearly Report object
	 * 
	 * @return YearlyReport_p 
	 */
	YearlyReport_p getYearlyReport();

	/**
	 * @brief Get the Yearly Report From Db object
	 * 
	 * @param year self-explanatory
	 * @return YearlyReport_p 
	 */
	YearlyReport_p getYearlyReportFromDb(int year);

	/**
	 * @brief Get the Investments Report object from Db
	 *
	*/
	YearlyInvestmentsReport_p getYearlyInvestmentsReport();

	/**
	 * @brief Get the Investments Report object from Db
	 *
	*/
	YearlyInvestmentsReport_p getYearlyInvestmentsReportFromDb(int year);

	/**
	 * @brief Push transaction in Database
	 * 
	 * @param month self-explanatory
	 * @param year self-explanatory
	 * @param t transaction
	 */
	void pushTransaction(int month, int year, Transaction_p t);

	/**
	 * @brief Get the Aggregate Category Report object from Db
	 * 
	 */
	MonthlyAggrCategoryReport_p getAggrCatReport(int month, int year);

	/**
	 * @brief Get the Aggregate Category Report object from Db without investments
	 * 
	 */
	MonthlyAggrCategoryReport_p getAggrCatReportWithoutInvestments(int month, int year);

	/**
	 * @brief Get the Config object from Database
	 * 
	 * @return config_t 
	 */
	Config_p getConfigFromDb();

    /**
	 * @brief Get the locally cached config data
	 *
	 * @return config_t
	 */
    Config_p getConfig();
};