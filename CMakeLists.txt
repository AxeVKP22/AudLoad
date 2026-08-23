cmake_minimum_required(VERSION 3.10)

project(AudLoad C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}")

add_library(glfw SHARED IMPORTED)

set_target_properties(glfw PROPERTIES
    IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/libs/gl/glfw/lib-vc2022/glfw3.lib"
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libs/gl/glfw/lib-vc2022/glfw3.dll"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/libs/gl/glfw/include"
)

add_library(glad STATIC
    libs/gl/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    libs/gl/glad/include
)

find_package(OpenGL REQUIRED)

set(SOURCES
    src/main.cpp
    src/audio/audio.cpp
    src/disk/load/load.cpp
    src/disk/save/save.cpp
    src/gui/gui/gui.cpp
    src/gui/init/init.cpp
    libs/imgui/imgui.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/backends/imgui_impl_glfw.cpp
    libs/imgui/backends/imgui_impl_opengl3.cpp
)

add_executable(AudLoad ${SOURCES})

target_include_directories(AudLoad PRIVATE
    libs/gl/glad/include
    libs/imgui
    libs/imgui/backends
    libs/json
    libs/miniaudio
    src
    libs/gl/glfw/include
)

target_link_libraries(AudLoad PRIVATE
    glad
    glfw
    OpenGL::GL
)

if(WIN32)
    target_compile_definitions(AudLoad PRIVATE
        _CRT_SECURE_NO_WARNINGS
    )
endif()

add_custom_command(TARGET AudLoad POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/libs/gl/glfw/lib-vc2022/glfw3.dll"
        "$<TARGET_FILE_DIR:AudLoad>/glfw3.dll"
)