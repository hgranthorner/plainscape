cmake_minimum_required(VERSION 3.23)
project(plainscape)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

add_subdirectory(vendor/glfw-3.3.8)
add_subdirectory(vendor/glad/src)
add_subdirectory(vendor/imgui-docking)

if (MSVC)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif ()

add_executable(plainscape src/main.cpp
        src/Model.cpp src/Model.h
        src/VertexArray.cpp src/VertexArray.h)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw Glad Imgui)
target_link_libraries(Imgui glfw)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        vendor/glfw-3.3.8/include
        vendor/glad/include
        vendor/glm-0.9.9.8
        vendor/imgui-docking
        vendor/imgui-docking/backends)
