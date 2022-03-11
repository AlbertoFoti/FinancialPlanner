#include "AssetAllocation.h"

AssetAllocation::AssetAllocation(std::shared_ptr<Core> core)
{
	this->core = core;
}

void AssetAllocation::Render()
{
    ImGui::Text("AssetAllocation...");
	
	ShowControlPanel("AssetAllocation Control Panel");
}

void AssetAllocation::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());
	ImGui::Text(panel_name.c_str());
	ImGui::End();
}