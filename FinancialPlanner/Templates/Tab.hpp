#pragma once

#include "imgui.h"
#include <string>
#include "../Core/Core.h"

/**
 * @brief Tab virtual interface (implemented by real tabs)
 * 
 */
class Tab {
public:
	/**
	 * @brief Rendering
	 * 
	 */
	virtual void Render() = 0;

	/**
	 * @brief Shows right control panel
	 * 
	 * @param panel_name self-explanatory
	 */
	virtual void ShowControlPanel(std::string panel_name) = 0;
};