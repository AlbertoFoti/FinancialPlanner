cmake_minimum_required (VERSION 3.8)

set(Sources 
    "FinancialPlanner.cpp" 
    "FinancialPlanner.hpp")

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

add_executable (${PROJECT_NAME} "main.cpp" ${Sources})

# sub directories
add_subdirectory("Core")
add_subdirectory("Templates")
add_subdirectory("Tabs")
add_subdirectory("Utility")
add_subdirectory("assets")
add_subdirectory("Database")

# Dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Templates)
target_link_libraries(${PROJECT_NAME} PRIVATE Tabs)
target_link_libraries(${PROJECT_NAME} PRIVATE Utility)

target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE implot::implot)
target_link_libraries(${PROJECT_NAME} PRIVATE jsoncpp_object jsoncpp_static JsonCpp::JsonCpp)