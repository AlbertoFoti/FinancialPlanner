#pragma once

#include "imgui.h"
#include "implot.h"
#include "../Core/Core.hpp"
#include "../Templates/Tab.hpp"

/**
 * @brief Manager Tab
 *
 */
class Manager : public Tab {
    /**
     * @brief Core component : Link with financial procedures and backend link
     *
     */
    std::shared_ptr<Core> core;

    /**
	 * @brief Temporary Data Structure to store accounts
	 *
	 */
    std::vector<Account_p> accounts;

    /**
	 * @brief Temporary Data Structure to store categories
	 *
	 */
    std::vector<Category_p> categories;

public:
    /**
     * @brief Construct a new Manager Tab
     *
     * @param core Core component : Link with financial procedures and backend link
     */
    Manager(std::shared_ptr<Core> core);

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
	 * @brief Panel View : shows account manager panel
	 *
	 */
    void ShowAccountManager();

    /**
	 * @brief Panel View : shows category manager panel
	 *
	 */
    void ShowCategoryManager();

    /**
     * @brief Panel View : shows investments category manager panel
     *
     */
    void ShowInvCategoryManager();

    /**
	 * @brief Panel View : shows compound interest calculator panel
	 *
	 * @param nameGUI Panel Name
	 */
    void ShowCompoundInterestCalculator(const char* nameGUI);
};