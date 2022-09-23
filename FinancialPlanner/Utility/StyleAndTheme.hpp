#pragma once
#include "../Core/Core.hpp"

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
 */
bool ShowStyleSelectorPLOT(const char* label);

/**
 * @brief Set Custom "Dark" Theme Colors
 *
 */
void SetDarkThemeColors();

/**
 * @brief Load Fonts
 *
 */
void loadFonts(std::shared_ptr<Core> core);

/**
 * @brief Set the Theme
 *
 */
void setTheme();

/**
 * @brief Set the Minimum Size of a Window
 *
 */
float setMinimumSizeWindow();