cmake_minimum_required (VERSION 3.8)

find_package(imgui CONFIG REQUIRED)

add_library(
	Templates
        Tab.hpp
    Tab.cpp)

#dependencies
target_link_libraries(Templates PRIVATE imgui::imgui)