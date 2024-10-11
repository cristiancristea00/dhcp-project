cmake_minimum_required(VERSION 3.19)
project(Fractal)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_COLOR_DIAGNOSTICS ON)

set(CMAKE_CXX_FLAGS "-Ofast -march=native -pipe -fno-builtin -Wfatal-errors -Wpedantic -Wall -Wextra")

add_executable(Fractal main.cpp Utils.cpp FractalGenerator.cpp MandelbrotGenerator.cpp JuliaGenerator.cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(Fractal ${OpenCV_LIBS})