cmake_minimum_required (VERSION 3.8)

find_package(imgui CONFIG REQUIRED)

add_subdirectory("Backend")

add_library(
	Core
		Core.hpp Core.cpp
)

target_link_libraries(Core PRIVATE imgui::imgui)
target_link_libraries(Core PRIVATE Backend)