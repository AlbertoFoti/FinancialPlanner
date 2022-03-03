#include "Forecasting.h"

Forecasting::Forecasting(Core* core)
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
	ImGui::Begin(panel_name.c_str());
	ImGui::Text(panel_name.c_str());
	ImGui::End();
}