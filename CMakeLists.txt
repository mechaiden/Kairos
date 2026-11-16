cmake_minimum_required(VERSION 3.20)
project(Kairos VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

option(KAIROS_BUILD_TESTS "Build the unit tests" ON)

# Warnings shared by every Kairos target.
add_library(kairos_warnings INTERFACE)
if(MSVC)
    target_compile_options(kairos_warnings INTERFACE /W4)
else()
    target_compile_options(kairos_warnings INTERFACE -Wall -Wextra -Wpedantic)
endif()

# Core library: all the maths, no input or output.
add_library(kairos_core
    src/math/distributions.cpp
    src/math/matrix.cpp
)
target_include_directories(kairos_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(kairos_core PRIVATE kairos_warnings)

# Command-line app.
add_executable(kairos_cli src/main.cpp)
target_link_libraries(kairos_cli PRIVATE kairos_core kairos_warnings)

if(KAIROS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
