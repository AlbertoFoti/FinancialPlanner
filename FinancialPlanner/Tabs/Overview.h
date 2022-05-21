#pragma once

#include "imgui.h"
#include "implot.h"
#include "../Core/Core.h"
#include "../Templates/Tab.h"
#include "../Utility/Utility.h"

/**
 * @brief Overview Tab
 * 
 */
class Overview : public Tab {
	/**
	 * @brief Core component : Link with financial procedures and backend link
	 * 
	 */
	std::shared_ptr<Core> core;

	/**
	 * @brief Temporary Data Structure to store NW-records
	 * 
	 */
	std::vector<NW_record_p> NW_records;

	/**
	 * @brief Temporary Data Structure to store account monthly records
	 * 
	 */
	AccountMonthlyDetails_p accountMonthlyRecords;
public:
	/**
	 * @brief Construct a new Overview Tab
	 * 
	 * @param core Core component : Link with financial procedures and backend link
	 */
	Overview(std::shared_ptr<Core> core);

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
};