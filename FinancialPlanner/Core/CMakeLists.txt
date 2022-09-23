cmake_minimum_required (VERSION 3.8)

find_package(imgui CONFIG REQUIRED)

add_library(
	Core
		Core.hpp
	Core.cpp
        Backend.hpp
	Backend.cpp
)

target_link_libraries(Core PRIVATE imgui::imgui)