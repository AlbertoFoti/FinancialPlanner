#include "NetWorth.h"

NetWorth::NetWorth(Core* core) {
	this->core = core;
}

void NetWorth::Render()
{
	ImGui::Text("Net Worth Tab");
	ImGui::Separator();

	if(ImGui::Button("Backend Test")) {
		this->core->testBackend();
	}

	ShowControlPanel("Net Worth Control Panel");
}

void NetWorth::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());
	ImGui::Text("Control Panel");
	ImGui::End();
}
