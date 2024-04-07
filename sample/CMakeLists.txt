cmake_minimum_required(VERSION 3.20)
project(samples)

add_executable(sample_matrix sample_matrix.cpp)
target_link_libraries(sample_matrix PUBLIC matrix)

add_executable(sample_SASolver sample_SASolver.cpp)
target_link_libraries(sample_SASolver PUBLIC optimization)