cmake_minimum_required(VERSION 3.0)
project(DaphneConfigCommandBuilder)

add_compile_options(-Wall)
add_executable(main main.cpp DaphneConfigCommandBuilder.cpp)
