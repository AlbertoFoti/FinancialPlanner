cmake_minimum_required (VERSION 3.8)

add_library(
	Tabs
	Overview.h
	Overview.cpp
	NetWorth.h
	NetWorth.cpp
	IncomeExpenses.h
	IncomeExpenses.cpp
	Investments.h
	Investments.cpp
	AssetAllocation.h
	AssetAllocation.cpp
	Forecasting.h
	Forecasting.cpp
	FIRE.h
	FIRE.cpp)

#dependencies

find_package(imgui CONFIG REQUIRED)
target_link_libraries(Tabs PRIVATE imgui::imgui)

#find_package(glfw3 CONFIG REQUIRED)
#target_link_libraries(Tabs PRIVATE glfw)

#find_package(glad CONFIG REQUIRED)
#target_link_libraries(Tabs PRIVATE glad::glad)

find_package(implot CONFIG REQUIRED)
target_link_libraries(Tabs PRIVATE implot::implot)