cmake_minimum_required (VERSION 3.8)

find_package(imgui CONFIG REQUIRED)

add_library(
        Backend
        Backend.hpp Backend.cpp
        Transaction.hpp
)

target_link_libraries(Backend PRIVATE imgui::imgui)
target_link_libraries(Backend PRIVATE Utility)