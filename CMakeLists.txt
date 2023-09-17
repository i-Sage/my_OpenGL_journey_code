cmake_minimum_required(VERSION 3.20..2.27)
project(Learning_OpenGL
        LANGUAGES C CXX
        DESCRIPTION "learing the how to use OpenGL and Computer Graphics"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(third_party)
add_subdirectory(app)
