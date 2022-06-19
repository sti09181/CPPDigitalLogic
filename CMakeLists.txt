##########################
##### CMakeLists.txt #####
##########################

cmake_minimum_required(VERSION 3.16)
project(CPPDigitalLogic)

file(GLOB SOURCE_FILES ${CMAKE_SOURCE_DIR}/Sources/*.c ${CMAKE_SOURCE_DIR}/Sources/*.cpp)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
