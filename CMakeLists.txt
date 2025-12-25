cmake_minimum_required(VERSION 3.15.0)

project(meow)

include_directories(inc/)

option(EXAMPLE "build an example binary" OFF)

if(EXAMPLE)
    add_subdirectory(example)
endif(EXAMPLE)

add_compile_options(-std=c99 -Wall -Wextra -Oz -fshort-enums -ffunction-sections -fdata-sections -fno-unroll-loops)
add_library(${PROJECT_NAME} src/meow.c)
target_include_directories(meow PUBLIC inc/)
