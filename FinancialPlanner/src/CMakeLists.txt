cmake_minimum_required (VERSION 3.8)


add_executable (main WIN32 "main.cpp" "FinancialPlanner.cpp" "FinancialPlanner.h" core templates tabs )

# Libraries
add_library(
	core
	"Core/Core.cpp"
	"Core/Core.h"
)

add_library(
	templates
	"Templates/Tab.cpp"
	"Templates/Tab.h"
)

add_library(
	tabs
	"Tabs/IncomeExpenses.cpp"
	"Tabs/IncomeExpenses.h"
	"Tabs/NetWorth.cpp"
	"Tabs/NetWorth.h"
)


# Dependencies

find_package(imgui CONFIG REQUIRED)
target_link_libraries(main PRIVATE imgui::imgui)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE glfw)

find_package(glad CONFIG REQUIRED)
target_link_libraries(main PRIVATE glad::glad)

find_package(implot CONFIG REQUIRED)
target_link_libraries(main PRIVATE implot::implot)