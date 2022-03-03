#include "FIRE.h"

FIRE::FIRE(Core* core)
{
	this->core = core;
}

void FIRE::Render()
{
    ImGui::Text("FIRE...");
	ShowControlPanel("FIRE Control Panel");
}

void FIRE::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());
	ImGui::Text(panel_name.c_str());
	ImGui::End();
}