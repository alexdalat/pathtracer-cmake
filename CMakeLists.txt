include(FetchContent)

cmake_minimum_required (VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

project(pathtracer-cmake)


FetchContent_Declare(
        imgui-glfw-glad-glm
        GIT_REPOSITORY https://github.com/cmmw/imgui-glfw-glad-glm.git
        GIT_TAG v4.1.0
)
FetchContent_MakeAvailable(imgui-glfw-glad-glm)

add_subdirectory(src)
