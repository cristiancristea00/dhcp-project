cmake_minimum_required(VERSION 3.19)
project(Fractal)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_COLOR_DIAGNOSTICS ON)

if(MSCV_IDE OR MSVC)
	set(CMAKE_CXX_FLAGS "/Ot /arch:AVX2 /fp:fast /EHsc /Zi")
else()
	set(CMAKE_CXX_FLAGS "-Ofast -march=native -pipe -fno-builtin -g")
endif()

add_executable(Fractal main.cpp Utils.cpp FractalGenerator.cpp MandelbrotGenerator.cpp JuliaGenerator.cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(Fractal ${OpenCV_LIBS})
