#include "FinancialPlanner.hpp"
#include "Tabs/Manager.hpp"

void FinancialPlanner::Init(GLFWwindow* window, const char* glsl_version)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Anti-Aliased plots
    ImPlot::GetStyle().AntiAliasedLines = true;
	
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

    // Core initialized
    core = std::make_shared<Core>();

    loadFonts();

    setTheme();

    // Accounts
    accounts = this->core->getAccountsFromDb();
    // Categories
    categories = this->core->getCategoriesFromDb();
}

void FinancialPlanner::Update() 
{
    // opt 
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Minimum window size (x coordinate), in dockspace
    float minWinSizeX = setMinimumSizeWindow();

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    // Window min size back to original value
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowMinSize.x = minWinSizeX;

    // borders and padding
    style.TabBorderSize = 1;
    style.TabRounding = 1;
    style.ItemSpacing.x = 10;
    style.ItemSpacing.y = 12;

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Window"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Config"))
        {
            ImGui::MenuItem("##CONFIG", NULL);
            ImGui::ShowFontSelector("Font");
            this->ShowStyleSelectorGUI("ImGui Style");
            this->ShowStyleSelectorPLOT("ImPlot Style");
            ImPlot::ShowColormapSelector("ImPlot Colormap");
            ImPlot::ShowInputMapSelector("Input Map");
            ImGui::Separator();
            ImGui::Checkbox("Anti-Aliased Lines", &ImPlot::GetStyle().AntiAliasedLines);
            ImGui::Checkbox("Use Local Time", &ImPlot::GetStyle().UseLocalTime);
            ImGui::Checkbox("Use ISO 8601", &ImPlot::GetStyle().UseISO8601);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("About"))
        {
            ImGui::MenuItem("\xA9 2022 Alberto Foti. All rights reserved", NULL);
            ImGui::Separator();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    // Your GUIs go Here !
    //this->ShowDemoWindow();
    //this->ShowDemoPlot();
    //this->ShowFontTesting();
    this->ShowMainView();

    ImGui::End();
}

void FinancialPlanner::Render() 
{
	// Render dear imgui into screen
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void FinancialPlanner::Shutdown() 
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

void FinancialPlanner::NewFrame()
{
	// feed inputs to dear imgui, start new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void FinancialPlanner::ShowMainView()
{
    ImGui::Begin("Financial Overview");

    // Views
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        if (ImGui::BeginTabItem("Overview"))
        {
            Overview overview_renderer = Overview(this->core);
            static long setup = 0;
            if( setup < 25 ) {
                setup++;
            } else {
                overview_renderer.Render();
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Net Worth"))
        {
            NetWorth nw_renderer = NetWorth(this->core);
            nw_renderer.Render();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Income/Expenses"))
        {
            IncomeExpenses ie_renderer = IncomeExpenses(this->core);
            ie_renderer.Render();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Investments"))
        {
            Investments inv_renderer = Investments(this->core);
            inv_renderer.Render();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Asset Allocation"))
        {
            AssetAllocation all_renderer = AssetAllocation(this->core);
            all_renderer.Render();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Forecasting"))
        {
            Forecasting for_renderer = Forecasting(this->core);
            for_renderer.Render();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("FIRE"))
        {
            FIRE fire_renderer = FIRE(this->core);
            fire_renderer.Render();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Manager"))
        {
            Manager manager = Manager(this->core);
            manager.Render();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

bool FinancialPlanner::ShowStyleSelectorGUI(const char* label)
{
    static int style_idx = -1;
    if (ImGui::Combo(label, &style_idx, "Navy\0Dark\0Light\0Classic\0"))
    {
        switch (style_idx)
        {
        case 0: ImGui::StyleColorsDark(); break;
        case 1: this->SetDarkThemeColors(); break;
        case 2: ImGui::StyleColorsLight(); break;
        case 3: ImGui::StyleColorsClassic(); break;
        }
        return true;
    }
    return false;
}

bool FinancialPlanner::ShowStyleSelectorPLOT(const char* label)
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

void FinancialPlanner::SetDarkThemeColors()
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

void FinancialPlanner::loadFonts()
{
    // Display size
    Config_p config = this->core->getConfigFromDb();

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

void FinancialPlanner::setTheme()
{
    // Custom Theme
    ImGui::StyleColorsDark();
    ImPlot::StyleColorsDark();
    ImPlotContext& gp = *GImPlot;
    gp.Style.Colormap = 7;
    //this->SetDarkThemeColors();
}

float FinancialPlanner::setMinimumSizeWindow()
{
    // Minimum window size (x coordinate), in dockspace
    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    float min_x = ImGui::GetWindowWidth()*0.20f;
    style.WindowMinSize.x = min_x;
    return minWinSizeX;
}

