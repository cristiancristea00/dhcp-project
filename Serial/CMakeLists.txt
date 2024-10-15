cmake_minimum_required(VERSION 3.19)
project(Fractal)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_COLOR_DIAGNOSTICS ON)

if(MSCV_IDE OR MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "/Ot /arch:AVX2 /fp:fast /EHsc /Zi")
	set(CMAKE_CXX_FLAGS_DEBUG "/Od /arch:AVX2 /fp:fast /EHsc /Zi /fsanitize=address")
else()
	set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -march=native -pipe -fno-builtin -g")
	set(CMAKE_CXX_FLAGS_DEBUG "-Og -march=native -pipe -fno-builtin -g -fsanitize=address,leak,undefined")
endif()

add_executable(Fractal main.cpp Utils.cpp FractalGenerator.cpp MandelbrotGenerator.cpp JuliaGenerator.cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(Fractal ${OpenCV_LIBS})
