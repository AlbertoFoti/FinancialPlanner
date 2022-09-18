#pragma once

#include "../Core/Core.h"
#include "../Templates/Tab.hpp"
#include "imgui.h"
#include "implot.h"

/**
 * @brief AssetAllocation Tab
 * 
 */
class AssetAllocation : public Tab {
	/**
	 * @brief Core component : Link with financial procedures and backend link
	 * 
	 */
	std::shared_ptr<Core> core;
public:
	/**
	 * @brief Construct a new AssetAllocation Tab
	 * 
	 * @param core Core component : Link with financial procedures and backend link
	 */
	AssetAllocation(std::shared_ptr<Core> core);

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
	 * @brief Asset Allocation Overview Tab
	 * 
	 */
	void ShowAssetAllocationOverview();

	/**
	 * @brief Asset Allocation Breakdown Tab
	 * 
	 */
	void ShowAssetAllocationBreakdown();

	/**
	 * @brief Asset Allocation Details Tab
	 * 
	 */
	void ShowAssetAllocationDetails();

private:
	/**
	 * @brief Asset Allocation Investment renderer
	 * 
	 * @param i 
	 */
	void AssetAllocationInvestment(int i, bool byPercentage, std::vector<double>& amounts_invested, std::vector<int>& perc_invested);
};