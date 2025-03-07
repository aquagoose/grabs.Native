cmake_minimum_required(VERSION 3.20)
project(grabs)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

add_library(grabs STATIC
        include/grabs/grabs.h
        src/grabs.c)

target_include_directories(grabs PUBLIC include)

if (WIN32)
    set(GRABS_SHARED_NAME "grabs.Native.dll")
    set(GRABS_RUNTIME_NAME "win-x64")
else()
    set(GRABS_SHARED_NAME "grabs.Native.so")
    set(GRABS_RUNTIME_NAME "linux-x64")
endif()

add_custom_command(
        TARGET grabs PRE_BUILD
        COMMAND dotnet publish -c Release -r ${GRABS_RUNTIME_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/grabs.Native/grabs.Native.csproj" -o ${CMAKE_CURRENT_SOURCE_DIR}/grabs.Native/bin/Release/net9.0/publish
        VERBATIM
)

add_custom_command(
        TARGET grabs POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/grabs.Native/bin/Release/net9.0/publish/" "${CMAKE_BINARY_DIR}"
        VERBATIM
)

add_executable(grabstest main.cpp)
target_link_libraries(grabstest PUBLIC grabs)
if (WIN32)
    find_package(Vulkan REQUIRED)
    target_include_directories(grabstest PUBLIC ${Vulkan_INCLUDE_DIR})
    target_link_libraries(grabstest PUBLIC C:/VulkanSDK/1.4.304.1/Lib/SDL2main.lib)
    target_link_libraries(grabstest PUBLIC C:/VulkanSDK/1.4.304.1/Lib/SDL2.lib)
else()
    target_link_libraries(grabstest PUBLIC SDL2)
endif()