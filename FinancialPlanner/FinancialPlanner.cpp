#include "FinancialPlanner.hpp"

void FinancialPlanner::Init(GLFWwindow* window, const char* glsl_version)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = "../share/financialplanner/imgui.ini";
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

    loadFonts(core);

    setTheme();

    // Accounts
    this->core->getAccountsFromDb();
    // Categories
    this->core->getCategoriesFromDb();
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
            ShowStyleSelectorGUI("ImGui Style");
            ShowStyleSelectorPLOT("ImPlot Style");
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

    this->ShowGUI();

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

void FinancialPlanner::ShowGUI() {
    //this->ShowDemoWindow();
    //this->ShowDemoPlot();
    //this->ShowFontTesting();
    this->ShowMainView();
}

void FinancialPlanner::ShowMainView()
{
    ImGui::Begin("Financial Overview");

    // Views
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_Reorderable;
    if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
    {
        static std::shared_ptr<Overview> overview_renderer  = std::make_shared<Overview>(this->core);
        static std::shared_ptr<NetWorth> net_worth_renderer = std::make_shared<NetWorth>(this->core);
        static std::shared_ptr<IncomeExpenses> income_expenses_renderer = std::make_shared<IncomeExpenses>(this->core);
        static std::shared_ptr<Investments> investments_renderer = std::make_shared<Investments>(this->core);
        static std::shared_ptr<AssetAllocation> asset_allocation_renderer = std::make_shared<AssetAllocation>(this->core);
        static std::shared_ptr<Forecasting> forecasting_renderer = std::make_shared<Forecasting>(this->core);
        static std::shared_ptr<FIRE> fire_renderer = std::make_shared<FIRE>(this->core);
        static std::shared_ptr<Manager> manager_renderer = std::make_shared<Manager>(this->core);

        static std::forward_list<std::pair<std::string, std::shared_ptr<Tab>>> labels = {
                { "Overview##overview", overview_renderer },
                { "Net Worth##networth", net_worth_renderer },
                { "Cash Flow##cashflow", income_expenses_renderer },
                { "Investments##investments", investments_renderer },
                { "Asset Allocation##assetallocation", asset_allocation_renderer },
                { "Forecasting##forecasting", forecasting_renderer },
                { "FIRE##fire", fire_renderer },
                { "Manager##manager", manager_renderer },
        };

        for (auto pair : labels) {
            if (ImGui::BeginTabItem(pair.first.c_str()))
            {
                pair.second->Render();
                ImGui::EndTabItem();
            }
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
}

