cmake_minimum_required(VERSION 2.8.12)
project(sitech-deiver)
add_definitions("-std=c++14")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(fake fake/main.cpp)
target_link_libraries(fake ${CONAN_LIBS})
