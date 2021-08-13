cmake_minimum_required(VERSION 3.19)
project(lab_project C)

set(CMAKE_C_STANDARD 90)

add_executable(lab_project main.c passManager.c commonFuncs.c directivesManager.c firstPassCommandsManager.c firstPass.c)