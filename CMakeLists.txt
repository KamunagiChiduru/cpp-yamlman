cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O2")
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

project(yamlman CXX)

include_directories(/usr/include/)
include_directories(/usr/include/c++/4.8.2/)

add_library(yamlman SHARED parser.cpp)
set_target_properties(yamlman PROPERTIES VERSION "0.0.1" SOVERSION "0.0.1")

target_link_libraries(yamlman yaml)

install(TARGETS yamlman LIBRARY DESTINATION lib)
install(FILES parser.h DESTINATION include)
install(FILES event.h DESTINATION include)
