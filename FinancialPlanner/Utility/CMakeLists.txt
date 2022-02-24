cmake_minimum_required (VERSION 3.8)

add_library(
	Utility
	Utility.h
	Utility.cpp
)

#dependencies

#find_package(imgui CONFIG REQUIRED)
#target_link_libraries(Utility PRIVATE imgui::imgui)

#find_package(glfw3 CONFIG REQUIRED)
#target_link_libraries(Utility PRIVATE glfw)

#find_package(glad CONFIG REQUIRED)
#target_link_libraries(Utility PRIVATE glad::glad)

#find_package(implot CONFIG REQUIRED)
#target_link_libraries(Utility PRIVATE implot::implot)

find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(Utility PRIVATE jsoncpp_lib jsoncpp_object JsonCpp::JsonCpp)