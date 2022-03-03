#include "Investments.h"

Investments::Investments(Core* core)
{
	this->core = core;
}

void Investments::Render()
{
    ImGui::Text("Investments...");
	ShowControlPanel("Investments Control Panel");
}

void Investments::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());
	ImGui::Text(panel_name.c_str());
	ImGui::End();
}