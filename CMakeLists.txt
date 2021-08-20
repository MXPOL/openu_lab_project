cmake_minimum_required(VERSION 3.19)

project(lab_project C)

set(CMAKE_C_STANDARD 90)

add_executable(untitled3 main.c passManager.c commonFuncs.c directivesManager.c firstPassCommandsManager.c firstPass.c firstPassFunctions.c outputManager.c secondPassManager.c secondPassCommandManager.c hexFunctions.c)
