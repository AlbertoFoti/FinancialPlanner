cmake_minimum_required (VERSION 3.8)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_library(
	Tabs
        Overview.hpp Overview.cpp
		NetWorth.hpp NetWorth.cpp
		IncomeExpenses.hpp IncomeExpenses.cpp
		Investments.hpp Investments.cpp
		AssetAllocation.hpp AssetAllocation.cpp
		Forecasting.hpp Forecasting.cpp
		FIRE.hpp FIRE.cpp
		Manager.hpp Manager.cpp
)

#dependencies

target_link_libraries(Tabs PRIVATE glad::glad)
target_link_libraries(Tabs PRIVATE glfw)
target_link_libraries(Tabs PRIVATE imgui::imgui)
#target_link_libraries(Tabs PRIVATE implot::implot)