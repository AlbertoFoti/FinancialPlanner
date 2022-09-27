#pragma once

#include <memory>
#include "imgui.h"

// Sub Accounts
struct SubAccount;
using SubAccount_p = std::shared_ptr<SubAccount>;

// Account
struct Account;
using Account_p = std::shared_ptr<Account>;

// Accounts detail Monthly record
struct AccountMonthlyRecord;
using AccountMonthlyRecord_p = std::shared_ptr<AccountMonthlyRecord>;

struct AccountsMonthlyDetail;
using AccountsMonthlyDetail_p = std::shared_ptr<AccountsMonthlyDetail>;

// Account Details Monthly Records
struct AccountMonthlyRecordComplex;
using AccountMonthlyRecordComplex_p = std::shared_ptr<AccountMonthlyRecordComplex>;

struct AccountMonthlyDetails;
using AccountMonthlyDetails_p = std::shared_ptr<AccountMonthlyDetails>;

// NW record
struct NW_record;
using NW_record_p = std::shared_ptr<NW_record>;

// Subcategory
struct SubCategory;
using SubCategory_p = std::shared_ptr<SubCategory>;

// Category
struct Category;
using Category_p = std::shared_ptr<Category>;

// Transaction
struct Transaction;
using Transaction_p = std::shared_ptr<Transaction>;

// Monthly Transactions Summary
struct MonthlyTransactions;
using MonthlyTransactions_p = std::shared_ptr<MonthlyTransactions>;

// Monthly Category Balance
struct MonthlyCategoryBalanceT;
using MonthlyCategoryBalanceT_p = std::shared_ptr<MonthlyCategoryBalanceT>;

// Monthly Subcategory Balance
struct MonthlySubCategoryBalanceT;
using MonthlySubCategoryBalanceT_p = std::shared_ptr<MonthlySubCategoryBalanceT>;

struct MonthlyAccountBalanceT;
using MonthlyAccountBalanceT_p = std::shared_ptr<MonthlyAccountBalanceT>;

struct MonthlyAggrAccountReport;
using MonthlyAggrAccountReport_p = std::shared_ptr<MonthlyAggrAccountReport>;

// Aggregate Subcategory balance
struct MonthlyAggrSubCategoryReport;
using MonthlyAggrSubCategoryReport_p = std::shared_ptr<MonthlyAggrSubCategoryReport>;

// Aggregate Category balance
struct MonthlyAggrCategoryReport;
using MonthlyAggrCategoryReport_p = std::shared_ptr<MonthlyAggrCategoryReport>;

// Reports
struct MonthlyReport;
using MonthlyReport_p = std::shared_ptr<MonthlyReport>;

struct YearlyReport;
using YearlyReport_p = std::shared_ptr<YearlyReport>;

// Investments Reports
struct MonthlyInvestmentsReport;
using MonthlyInvestmentsReport_p = std::shared_ptr<MonthlyInvestmentsReport>;

struct YearlyInvestmentsReport;
using YearlyInvestmentsReport_p = std::shared_ptr<YearlyInvestmentsReport>;

struct AllTimeInvestmentsReport;
using AllTimeInvestmentsReport_p = std::shared_ptr<AllTimeInvestmentsReport>;

struct Config_t;
using Config_p = std::shared_ptr<Config_t>;

#include "Colors.hpp"
#include "CustomTime.hpp"
#include "CustomSort.hpp"
#include "Custom_Widgets.hpp"
#include "Plotter.hpp"
#include "StyleAndTheme.hpp"