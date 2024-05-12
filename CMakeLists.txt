cmake_minimum_required(VERSION 3.0)
project(DaphneConfigCommandBuilder)

add_compile_options(-Wall)
add_executable(daphne_config main.c DaphneConfig.c CommandBuilder.c CommandBuilderHelpers.c SerialPort.c)

add_executable(serial_port_test SerialPortTest.c SerialPort.c)
add_executable(command_builder_test CommandBuilderTest.cpp CommandBuilder.c CommandBuilderHelpers.c)
add_executable(daphne_config_test DaphneConfigTest.c DaphneConfig.c)