#pragma once

#include "imgui.h"
#include "implot.h"
#include "../Core/Core.h"
#include "../Templates/Tab.h"

/**
 * @brief IncomeExpenses Tab
 * 
 */
class IncomeExpenses : public Tab {
	/**
	 * @brief Core component : Link with financial procedures and backend link
	 * 
	 */
	Core* core;

	/**
	 * @brief Temporary Data Structure to store monthly transactions
	 * 
	 */
	MonthlyTransactions_p monthlyTransactions;

	/**
	 * @brief Temporary Data Structure to store yearly reports
	 * 
	 */
	YearlyReport_p YearlyReport;
public:
	/**
	 * @brief Construct a new IncomeExpenses Tab
	 * 
	 * @param core Core component : Link with financial procedures and backend link
	 */
	IncomeExpenses(Core* core);

	/**
	 * @brief Rendering
	 * 
	 */
	void Render();

	/**
	 * @brief Show Right Control Panel
	 * 
	 * @param panel_name self-explanatory
	 */
	void ShowControlPanel(std::string panel_name);

	/**
	 * @brief TODO
	 * 
	 * @param i 
	 * @param month 
	 * @param year 
	 */
	void ShowEditTransactionPanel(int i, int month, int year);

	/**
	 * @brief Income Expenses : Overview (aggregate) Tab
	 * 
	 */
	void ShowIncomeExpensesAggregate();

	/**
	 * @brief Income Expenses : Details Tab
	 * 
	 */
	void IncomeExpenses::ShowIncomeExpensesDetails();
};