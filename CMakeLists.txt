cmake_minimum_required(VERSION 3.16.3)
project(sitech-driver)
set(CMAKE_CXX_STANDARD 20)
#TODO: This is suggested by conan documentatio;  not sure if it's redundant
add_definitions("-std=c++20")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(common)
add_subdirectory(fake)
