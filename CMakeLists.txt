cmake_minimum_required(VERSION 3.0)
project(DaphneConfigCommandBuilder)

add_compile_options(-Wall)
add_executable(daphne_config_command_builder main.cpp CommandBuilder.cpp CommandBuilderHelpers.cpp ConfigParser.cpp log.cpp)

add_executable(serial_port_test SerialPortTest.c SerialPort.c)