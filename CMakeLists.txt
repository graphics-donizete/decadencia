cmake_minimum_required(VERSION 3.10.0)
project(decadencia VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)

include(skia.cmake)
add_subdirectory(external/SFML)

add_executable(decadencia main.cpp)
target_link_libraries(decadencia libskia SFML::Graphics)