cmake_minimum_required(VERSION 3.20)
project(test)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test_matrix test_matrix.cpp)
target_link_libraries(test_matrix gtest_main matrix)
add_test(NAME test_matrix COMMAND test_matrix)
