#include "NetWorth.h"

NetWorth::NetWorth(Core* core) {
	this->core = core;
}

void NetWorth::Render()
{
	ImGui::Text("Net Worth Tab");
	ImGui::Separator();
	
	static std::string test_string = "";
	if(ImGui::Button("Backend Test")) {
		test_string = this->core->testBackend();
	}
	ImGui::Text(test_string.c_str());

	ShowControlPanel("Net Worth Control Panel");
}

void NetWorth::ShowControlPanel(std::string panel_name)
{
	ImGui::Begin(panel_name.c_str());
	ImGui::Text("Control Panel");
	ImGui::End();
}
