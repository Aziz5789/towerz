cmake_minimum_required(VERSION 3.24)
project(towerz C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.4.20
    GIT_SHALLOW    TRUE)
FetchContent_MakeAvailable(SDL3)

add_executable(towerz src/main.c)

target_link_libraries(towerz PRIVATE SDL3::SDL3)

if(NOT MSVC)
    target_compile_options(towerz PRIVATE -Wall -Wextra)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(towerz PRIVATE -fsanitize=address,undefined -g)
        target_link_options(towerz PRIVATE -fsanitize=address,undefined)
    endif()
endif()
