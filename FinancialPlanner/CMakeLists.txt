cmake_minimum_required (VERSION 3.8)

set(Sources 
    "FinancialPlanner.cpp" 
    "FinancialPlanner.h")

add_executable (main "main.cpp" ${Sources})

# sub directories
add_subdirectory("Core")
add_subdirectory("Templates")
add_subdirectory("Tabs")
add_subdirectory("Utility")

# Dependencies

target_link_libraries(main PRIVATE Core)
target_link_libraries(main PRIVATE Templates)
target_link_libraries(main PRIVATE Tabs)
target_link_libraries(main PRIVATE Utility)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(main PRIVATE imgui::imgui)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE glfw)

find_package(glad CONFIG REQUIRED)
target_link_libraries(main PRIVATE glad::glad)

find_package(implot CONFIG REQUIRED)
target_link_libraries(main PRIVATE implot::implot)

find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(main PRIVATE jsoncpp_object jsoncpp_static JsonCpp::JsonCpp)