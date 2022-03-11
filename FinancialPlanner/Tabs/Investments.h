#pragma once

#include "imgui.h"
#include "implot.h"
#include "../Templates/Tab.h"
#include "Plotter.h"

/**
 * @brief Investments Tab
 * 
 */
class Investments : public Tab {
	/**
	 * @brief Core component : Link with financial procedures and backend link
	 * 
	 */
	std::shared_ptr<Core> core;
public:
	/**
	 * @brief Construct a new Investments Tab
	 * 
	 * @param core Core component : Link with financial procedures and backend link
	 */
	Investments(std::shared_ptr<Core> core);

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
	 * @brief Investments Overview Tab
	 * 
	 */
	void ShowInvestmentsOverview();

	/**
	 * @brief Investments Breakdown Tab
	 * 
	 */
	void ShowInvestmentsBreakdown();

	/**
	 * @brief Investments Details Tab
	 * 
	 */
	void ShowInvestmentsDetails();
};