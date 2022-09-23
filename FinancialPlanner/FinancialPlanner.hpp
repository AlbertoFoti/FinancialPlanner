#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "Tabs/Manager.hpp"
#include "Core/Core.hpp"
#include "Tabs/IncomeExpenses.hpp"
#include "Tabs/NetWorth.hpp"
#include "Tabs/Overview.hpp"
#include "Tabs/Investments.hpp"
#include "Tabs/AssetAllocation.hpp"
#include "Tabs/Forecasting.hpp"
#include "Tabs/FIRE.hpp"

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
	 * @brief ImGui and ImPlot context Shutdown, rendering API cleanup.
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
     * @brief Function to display all custom GUIs
     */
    void ShowGUI();

	/**
	 * @brief Central View Rendering
	 * 
	 */
	void ShowMainView();
};
