#include "NetWorth.h"

void NetWorth::Render()
{
	ImGui::Text("Net Worth Tab");

	ShowControlPanel("Net Worth Control Panel");
}

void NetWorth::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());
	ImGui::Text("Control Panel");
	ImGui::End();
}
