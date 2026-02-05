cmake_minimum_required(VERSION 3.15)
project(ECS_System)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(${PROJECT_NAME} src/main.cpp
        include/core/resource_handle.h
        include/core/resource_container.h)

target_include_directories(${PROJECT_NAME} PRIVATE
    include
    include/core
    include/ecs
    include/ecs/components
    include/rendering
)

add_subdirectory(libs/glfw)
add_subdirectory(libs/glm)
add_library(glad libs/glad/src/glad.c)

target_include_directories(glad PUBLIC
  libs/glad/include
)

target_include_directories(${PROJECT_NAME} PUBLIC
  libs/glad/include
  libs/glfw/include
  libs/glm
  libs/stb_image
)

target_sources(${PROJECT_NAME} PRIVATE
  src/core/window.cpp
  src/ecs/entity_manager.cpp
  src/stb_image.cpp
  src/rendering/rendering_system.cpp
  src/rendering/shader.cpp
)

target_link_libraries(${PROJECT_NAME} glad glfw glm::glm)
