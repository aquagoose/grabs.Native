cmake_minimum_required(VERSION 3.20)
project(grabs LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

add_library(grabs STATIC
        include/grabs/grabs.h
        src/grabs.c)

target_include_directories(grabs PUBLIC include)

add_custom_command(
        TARGET grabs PRE_BUILD
        COMMAND dotnet publish -c Release --use-current-runtime "${CMAKE_CURRENT_SOURCE_DIR}/grabs.Native/grabs.Native.csproj"
        VERBATIM
)

if (WIN32)
    set(GRABS_SHARED_NAME "grabs.Native.dll")
else()
    set(GRABS_SHARED_NAME "grabs.Native.so")
endif()

add_custom_command(
        TARGET grabs POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_SOURCE_DIR}/grabs.Native/bin/Release/net9.0/linux-x64/publish/${GRABS_SHARED_NAME}" "${CMAKE_BINARY_DIR}"
        VERBATIM
)

add_executable(grabstest main.cpp)
target_link_libraries(grabstest PUBLIC grabs)
target_link_libraries(grabstest PUBLIC SDL2)