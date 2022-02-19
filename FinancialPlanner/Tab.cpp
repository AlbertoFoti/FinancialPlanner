#include "Tab.h"

void Tab::Render()
{
	ImGui::Text("Generic Tab");
}

void Tab::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());
	ImGui::Text("Control Panel : %s", panel_name.c_str());
	ImGui::End();
}
