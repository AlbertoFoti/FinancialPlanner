#include "Forecasting.hpp"

Forecasting::Forecasting(std::shared_ptr<Core> core)
{
	this->core = core;
}

void Forecasting::Render()
{
    ImGui::Text("Forecasting...");
	
	ShowControlPanel("Forecasting Control Panel");
}

void Forecasting::ShowControlPanel(std::string panel_name)
{
    static char c_str[50];
    sprintf(c_str, "%s", panel_name.c_str());

	ImGui::Begin(c_str);
	ImGui::Text("%s", c_str);
	ImGui::End();
}