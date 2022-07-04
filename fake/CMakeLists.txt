
add_executable(fake src/main.cpp)
target_include_directories(fake PRIVATE include/)
target_link_libraries(fake ${CONAN_LIBS} common)
