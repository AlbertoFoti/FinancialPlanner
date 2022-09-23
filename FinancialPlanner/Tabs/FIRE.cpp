#include "FIRE.hpp"

FIRE::FIRE(std::shared_ptr<Core> core)
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
    static char c_str[50];
    sprintf(c_str, "%s", panel_name.c_str());

    ImGui::Begin(c_str);
    ImGui::Text("%s", c_str);
	ImGui::End();
}