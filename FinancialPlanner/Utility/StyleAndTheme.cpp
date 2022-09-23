#include "StyleAndTheme.hpp"
#include "imgui.h"
#include "implot.h"
#include "implot_internal.h"
#include "Utility.hpp"
#include <memory>

bool ShowStyleSelectorGUI(const char* label)
{
    static int style_idx = -1;
    if (ImGui::Combo(label, &style_idx, "Navy\0Dark\0Light\0Classic\0"))
    {
        switch (style_idx)
        {
            case 0: ImGui::StyleColorsDark(); break;
            case 1: SetDarkThemeColors(); break;
            case 2: ImGui::StyleColorsLight(); break;
            case 3: ImGui::StyleColorsClassic(); break;
        }
        return true;
    }
    return false;
}

bool ShowStyleSelectorPLOT(const char* label)
{
    static int style_idx = -1;
    if (ImGui::Combo(label, &style_idx, "Navy\0Dark\0Light\0Classic\0"))
    {
        switch (style_idx)
        {
            case 0: ImPlot::StyleColorsAuto(); break;
            case 1: ImPlot::StyleColorsDark(); break;
            case 2: ImPlot::StyleColorsLight(); break;
            case 3: ImPlot::StyleColorsClassic(); break;
        }
        return true;
    }
    return false;
}

void SetDarkThemeColors()
{
    auto& colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Text
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

    // Menu
    colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    // Nav
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // others
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);

}

void loadFonts(std::shared_ptr<Core> core)
{
    // Display size
    Config_p config = core->getConfigFromDb();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Fonts
    if(config->display_resolution == "1920x1080"){
        // 0
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Bold.ttf", 11.0f);
        // 1
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Bold.ttf", 15.0f);
        // 2
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Bold.ttf", 20.0f);
        // 3
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Bold.ttf", 33.0f);
        // 4
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Medium.ttf", 11.0f);
        // 5
        if(config->default_font == "Roboto") {
            io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Medium.ttf", 15.0f);
        } else {
            io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Medium.ttf", 15.0f);
        }
        // 6
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Medium.ttf", 20.0f);
        // 7
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Medium.ttf", 33.0f);
        // 8
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Thin.ttf", 11.0f);
        // 9
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Thin.ttf", 15.0f);
        // 10
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Thin.ttf", 20.0f);
        // 11
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Thin.ttf", 33.0f);
        // 12
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProHeavy/BlenderProHeavy.ttf", 11.0f);
        // 13
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProHeavy/BlenderProHeavy.ttf", 14.0f);
        // 14
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProHeavy/BlenderProHeavy.ttf", 20.0f);
        // 15
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProHeavy/BlenderProHeavy.ttf", 33.0f);
        // 16
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProMedium/BlenderProMedium.ttf", 11.0f);
        // 17
        if(config->default_font == "Blender") {
            io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProMedium/BlenderProMedium.ttf", 14.0f);
        } else {
            io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProMedium/BlenderProMedium.ttf", 14.0f);
        }
        // 18
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProMedium/BlenderProMedium.ttf", 20.0f);
        // 19
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProMedium/BlenderProMedium.ttf", 33.0f);
        // 20
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProThin/BlenderProThin.ttf", 11.0f);
        // 21
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProThin/BlenderProThin.ttf", 14.0f);
        // 22
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProThin/BlenderProThin.ttf", 20.0f);
        // 23
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProThin/BlenderProThin.ttf", 33.0f);
        // 24
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProThin/BlenderProThin.ttf", 33.0f);
        // 25
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProThin/BlenderProThin.ttf", 33.0f);
        // 26
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProHeavy/BlenderProHeavy.ttf", 33.0f);
    } else {
        // 0
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Bold.ttf", 11.0f);
        // 1
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Bold.ttf", 15.0f);
        // 2
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Bold.ttf", 20.0f);
        // 3
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Bold.ttf", 33.0f);
        // 4
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Medium.ttf", 11.0f);
        // 5
        if(config->default_font == "Roboto") {
            io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Medium.ttf", 15.0f);
        } else {
            io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Medium.ttf", 15.0f);
        }
        // 6
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Medium.ttf", 20.0f);
        // 7
        io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Medium.ttf", 33.0f);
        // 8
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Thin.ttf", 11.0f);
        // 9
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Thin.ttf", 15.0f);
        // 10
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Thin.ttf", 20.0f);
        // 11
        io.Fonts->AddFontFromFileTTF("assets/fonts/Roboto/Roboto-Thin.ttf", 33.0f);
        // 12
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProHeavy/BlenderProHeavy.ttf", 11.0f);
        // 13
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProHeavy/BlenderProHeavy.ttf", 14.0f);
        // 14
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProHeavy/BlenderProHeavy.ttf", 20.0f);
        // 15
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProHeavy/BlenderProHeavy.ttf", 33.0f);
        // 16
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProMedium/BlenderProMedium.ttf", 11.0f);
        // 17
        if(config->default_font == "Blender") {
            io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProMedium/BlenderProMedium.ttf", 14.0f);
        } else {
            io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProMedium/BlenderProMedium.ttf", 14.0f);
        }
        // 18
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProMedium/BlenderProMedium.ttf", 20.0f);
        // 19
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProMedium/BlenderProMedium.ttf", 33.0f);
        // 20
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProThin/BlenderProThin.ttf", 11.0f);
        // 21
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProThin/BlenderProThin.ttf", 14.0f);
        // 22
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProThin/BlenderProThin.ttf", 20.0f);
        // 23
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProThin/BlenderProThin.ttf", 33.0f);
        // 24
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProThin/BlenderProThin.ttf", 70.0f);
        // 25
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProMedium/BlenderProMedium.ttf", 70.0f);
        // 26
        io.Fonts->AddFontFromFileTTF("assets/fonts/Blender/BlenderProHeavy/BlenderProHeavy.ttf", 70.0f);
    }
}

void setTheme()
{
    // Custom Theme
    ImGui::StyleColorsDark();
    ImPlot::StyleColorsDark();
    ImPlotContext& gp = *GImPlot;
    gp.Style.Colormap = 7;
    //this->SetDarkThemeColors();
}

float setMinimumSizeWindow()
{
    // Minimum window size (x coordinate), in dockspace
    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    float min_x = ImGui::GetWindowWidth()*0.20f;
    style.WindowMinSize.x = min_x;
    return minWinSizeX;
}