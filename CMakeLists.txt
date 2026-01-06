cmake_minimum_required(VERSION 3.15)
project(ECS_System)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE include)

# target_sources(${PROJECT_NAME} PRIVATE)