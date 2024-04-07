cmake_minimum_required(VERSION 3.0)
project(DaphneConfigCommandBuilder)

add_compile_options(-Wall)
add_executable(daphne_config_command_builder main.cpp DaphneConfigCommandBuilder.cpp)
