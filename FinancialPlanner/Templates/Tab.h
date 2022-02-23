#pragma once

#include "imgui.h"
#include <string>
#include "../Core/Core.h"

class Tab {
public:
	virtual void Render() = 0;
	virtual void ShowControlPanel(std::string panel_name) = 0;
};