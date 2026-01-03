cmake_minimum_required(VERSION 3.28)
project(DicewarePlayer)

set(CMAKE_CXX_STANDARD 20)
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

find_package(Boost REQUIRED COMPONENTS unit_test_framework)
enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
