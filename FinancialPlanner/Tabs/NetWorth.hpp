#pragma once

#include "imgui.h"
#include "implot.h"
#include "../Core/Core.h"
#include "../Templates/Tab.hpp"
#include "../Utility/Utility.hpp"
#include <string>

/**
 * @brief Net Worth Tab
 * 
 */
class NetWorth : public Tab {
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
public:
	/**
	 * @brief Construct a new Net Worth Tab
	 * 
	 * @param core Core component : Link with financial procedures and backend link
	 */
	NetWorth(std::shared_ptr<Core> core);

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