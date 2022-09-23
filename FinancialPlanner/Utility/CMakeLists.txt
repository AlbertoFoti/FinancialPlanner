cmake_minimum_required (VERSION 3.8)

find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)

add_library(
	Utility
	"Utility.hpp"
	"CustomTime.hpp"
	"CustomTime.cpp"
	"CustomSort.hpp"
	"CustomSort.cpp"
	"Custom_Widgets.hpp"
	"Custom_Widgets.cpp"
	"Plotter.hpp"
	"Plotter.cpp"
	"namespace_declarations.hpp"
)

target_link_libraries(Utility PRIVATE imgui::imgui)
target_link_libraries(Utility PRIVATE implot::implot)