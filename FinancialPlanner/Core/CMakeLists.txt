cmake_minimum_required (VERSION 3.8)

add_library(
	Core
	Core.h
	Core.cpp
	Backend.h
	Backend.cpp
)

find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(Core PRIVATE jsoncpp_object jsoncpp_static JsonCpp::JsonCpp)