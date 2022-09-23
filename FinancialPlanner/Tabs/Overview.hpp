#pragma once

#include "imgui.h"
#include "implot.h"
#include "../Core/Core.hpp"
#include "../Templates/Tab.hpp"
#include "../Utility/Utility.hpp"

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

    /**
    * @brief Temporary Data Structure to store accounts
    *
    */
    std::vector<Account_p> accounts;
public:
	/**
	 * @brief Construct a new Overview Tab
	 * 
	 * @param core Core component : Link with financial procedures and backend link
	 */
	explicit Overview(std::shared_ptr<Core> core);

	/**
	 * @brief Rendering
	 * 
	 */
	void Render() override;

	/**
	 * @brief Show Right Control Panel
	 * 
	 * @param panel_name self-explanatory
	 */
	void ShowControlPanel(std::string panel_name) override;

    /**
    * @brief Panel View : shows account manager panel
    *
    */
    void ShowAccountManager();
};