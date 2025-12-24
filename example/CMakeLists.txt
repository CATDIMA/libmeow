cmake_minimum_required(VERSION 3.15.0)

project("meow_example")
set(CMAKE_CXX_STANDARD 17)

include_directories(../inc/)
link_directories(../build)

add_executable(${PROJECT_NAME} stdout_meow.cpp)
target_link_libraries(${PROJECT_NAME} libmeow.a)