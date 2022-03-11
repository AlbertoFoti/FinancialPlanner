#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include <iostream>
#include <string>

#include "Core/Core.h"
#include "Tabs/IncomeExpenses.h"
#include "Tabs/NetWorth.h"
#include "Tabs/Overview.h"
#include "Tabs/Investments.h"
#include "Tabs/AssetAllocation.h"
#include "Tabs/Forecasting.h"
#include "Tabs/FIRE.h"

/**
 * @brief Financial Planner : Main component managing views update and rendering
 * 
 */
class FinancialPlanner 
{
private:
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
	 * @brief Sets up ImGui and ImPlot context, loads fonts, assets and themes. 
	 * Initializes graphical components through some rendering API (ex. glfw/opengl3/glad, Vulkan, DirectX)
	 * 
	 * @param window self-explanatory
	 * @param glsl_version self-explanatory
	 */
	void Init(GLFWwindow *window, const char* glsl_version);

	/**
	 * @brief Update
	 * 
	 */
	virtual void Update();

	/**
	 * @brief Docking Space Setup, Views loading. Rendering.
	 * 
	 */
	void Render();

	/**
	 * @brief Shutsdown ImGui and ImPlot context, rendering API cleanup.
	 * 
	 */
	void Shutdown();

	/**
	 * @brief Creates a New Frame
	 * 
	 */
	void NewFrame();

private:
	/**
	 * @brief Central View Rendering
	 * 
	 */
	void ShowMainView();

	/**
	 * @brief Left Panel View : shows compound interest calculator panel
	 * 
	 * @param nameGUI Panel Name
	 */
	void ShowCompoundInterestCalculator(const char* nameGUI);

	/**
	 * @brief Left Panel View : shows account manager panel
	 * 
	 */
	void ShowAccountManager();

	/**
	 * @brief Left Panel View : shows category manager panel
	 * 
	 */
	void ShowCategoryManager();

	/**
	 * @brief Demo View : ImGui Demo panel
	 * 
	 */
	void ShowDemoWindow();

	/**
	 * @brief Demo View : ImPlot Demo panel
	 * 
	 */
	void ShowDemoPlot();

	/**
	 * @brief Demo View : Fonts Testing panel
	 * 
	 */
	void ShowFontTesting();

	/**
	 * @brief Utility : ImGui Configuration Options/Settings
	 * 
	 * @param label 
	 * @return ignore 
	 * @return ignore 
	 */
	bool ShowStyleSelectorGUI(const char* label);

	/**
	 * @brief Utility : ImPlot Configuration Options/Settings
	 * 
	 * @param label 
	 * @return ignore 
	 * @return ignore 
	 */
	bool ShowStyleSelectorPLOT(const char* label);

	/**
	 * @brief Set Custom "Dark" Theme Colors
	 * 
	 */
	void SetDarkThemeColors();

	/**
	 * @brief Utility : shows Helper Marker
	 * 
	 * @param desc 
	 */
	void HelpMarker(const char* desc);
};
