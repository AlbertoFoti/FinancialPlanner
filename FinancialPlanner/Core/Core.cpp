#include "Core.h"
#include "Core.h"
#include "Core.h"
#include "Core.h"
#include "Core.h"
#include "Core.h"
#include "Core.h"
#include "Core.h"
#include "Core.h"
#include "Core.h"
#include "Core.h"
#include "Core.h"
#include "Core.h"
#include "Core.h"

Core::Core()
{
	this->back_end.init();

	double from = getUNIXtime(1, 2019);
	double to = getUNIXtime(1, 2022);

	this->NW_records = this->back_end.getNWdata(from, to);
	this->MonthlyReport = this->getMonthlyTransactionsReportFromDb(1, 2021);
	this->YearlyReport = this->getYearlyReportFromDb(2021);
	//this->monthlyAccountsReport = this->getMonthlyAccountsReportFromDb(1, 2021);
}

float Core::CompoundInterestCalculate(float initialNW, float interestRate, float annualDeposits, int investmentYears, float* y_data)
{
	static int i = 0;

	if (investmentYears < 0 || interestRate < 0 || initialNW < 0) {
		return 0.00;
	}

	y_data[i] = initialNW;

	if (investmentYears > 0) {
		i++;
		float curr_nw = initialNW + annualDeposits + ((initialNW + annualDeposits) * (interestRate / 100));
		return this->CompoundInterestCalculate(curr_nw, interestRate, annualDeposits, investmentYears - 1, &y_data[0]);
	}
	else {
		i = 0;
		return initialNW;
	}

	return 0.00;
}

// Account =================================================================

std::vector<Account_p> Core::getAccounts()
{
	return this->accounts;
}

std::vector<Account_p> Core::getAccountsFromDb()
{
	this->accounts = this->back_end.getAccounts();
	return this->accounts;
}

void Core::pushAccount(Account_p x)
{
	this->back_end.pushAccount(x);
}

// Categories =====================================

std::vector<Category_p> Core::getCategories()
{
	return this->categories;
}

std::vector<Category_p> Core::getCategoriesFromDb()
{
	this->categories = this->back_end.getCategories();
	return this->categories;
}

std::vector<SubCategory_p> Core::getSubCategoriesOf(std::string catName)
{
	std::vector<SubCategory_p> x;
	for (Category_p c : this->categories) {
		if (c->Name == catName) {
			for (SubCategory_p s : c->subCategories) {
				x.push_back(s);
			}
		}
	}
	return x;
}

void Core::pushCategory(Category_p x)
{
	this->back_end.pushCategory(x);
}

void Core::pushSubCategory(std::string categoryName, SubCategory_p x)
{
	this->back_end.pushSubCategory(categoryName, x);
}

bool Core::checkCategoryExists(std::string name)
{
	std::vector<Category_p> categories = this->getCategoriesFromDb();
	for (Category_p x : categories) {
		if (x->Name == name) return true;
	}
	return false;
}

bool Core::checkErrors(std::string cat, std::string subCat, std::string type, double amount, int year)
{
	// Check Errors : To do
	return true;
}

// Net Worth ===============================================================

std::vector<NW_record_p> Core::getNWdata()
{
	return this->NW_records;
}

std::vector<NW_record_p> Core::getNWdataFromDb(double from, double to)
{
	this->NW_records = this->back_end.getNWdata(from, to);
	return this->NW_records;
}

MonthlyTransactions_p Core::getMonthlyTransactionsReport()
{
	return this->MonthlyReport;
}

MonthlyTransactions_p Core::getMonthlyTransactionsReportFromDb(int month, int year)
{
	this->MonthlyReport = this->back_end.getMonthlyReport(month, year);
	return this->MonthlyReport;
}

YearlyReport_p Core::getYearlyReport()
{
	return this->YearlyReport;
}

YearlyReport_p Core::getYearlyReportFromDb(int year)
{
	this->YearlyReport = this->back_end.getYearlyReport(year);
	return this->YearlyReport;
}

void Core::pushTransaction(int month, int year, Transaction_p t)
{
	// Push Transaction
	this->back_end.pushTransaction(month, year, t);

	// Update Accounts Details Data
	this->back_end.updateAccountsDetailsData(month, year, t);
	// Update category and subcategory Details Data
}

std::string Core::testBackend() 
{
	return back_end.sayHello();
}
