cmake_minimum_required(VERSION 3.20)
project(grabs LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

add_library(grabs STATIC
        include/grabs/grabs.h
        src/grabs.c)

target_include_directories(grabs PUBLIC include)

add_executable(grabstest main.cpp)
target_link_libraries(grabstest PUBLIC grabs)