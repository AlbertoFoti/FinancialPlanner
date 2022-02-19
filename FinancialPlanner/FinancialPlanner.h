#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include <iostream>
#include <string>

#include "Core.h"
#include "IncomeExpenses.h"
#include "NetWorth.h"

// Financial Planner
class FinancialPlanner {
private:
	Core core;
public:
	void Init(GLFWwindow *window, const char* glsl_version);
	virtual void Update();
	void Render();
	void Shutdown();

	void NewFrame();

private:
	// Views
	void ShowCompoundInterestCalculator(const char* nameGUI);
	void ShowDemoWindow();
	void ShowDemoPlot();

	// Main View
	void ShowMainView();

	// Theme
	void SetDarkThemeColors();

	// Helper Functions
	void HelpMarker(const char* desc);
};
