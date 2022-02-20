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

// Financial Planner
class FinancialPlanner {
private:
	Core* core;

	NetWorth* nw_renderer;
	IncomeExpenses* ie_renderer;
public:
	void Init(GLFWwindow *window, const char* glsl_version);
	virtual void Update();
	void Render();
	void Shutdown();

	void NewFrame();

private:
	// Main View
	void ShowMainView();

	// Other Views
	void ShowCompoundInterestCalculator(const char* nameGUI);
	void ShowAccountManager();

	// Demo Views
	void ShowDemoWindow();
	void ShowDemoPlot();

	// Theme
	void SetDarkThemeColors();

	// Helper Functions
	void HelpMarker(const char* desc);
};
