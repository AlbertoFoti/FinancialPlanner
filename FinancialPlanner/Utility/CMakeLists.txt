cmake_minimum_required (VERSION 3.8)

add_library(
	Utility
	"Utility.h"
	"CustomTime.h"
	"CustomTime.cpp"
	"CustomSort.h"
	"CustomSort.cpp"
	"Custom_Widgets.h"
	"Custom_Widgets.cpp"
	"Plotter.h"
	"Plotter.cpp"
)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(Utility PRIVATE imgui::imgui)

find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(Utility PRIVATE jsoncpp_lib jsoncpp_object JsonCpp::JsonCpp)

find_package(implot CONFIG REQUIRED)
target_link_libraries(Utility PRIVATE implot::implot)