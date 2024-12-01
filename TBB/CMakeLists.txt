cmake_minimum_required(VERSION 3.19)
project("Parallel Fractal Generator using TBB" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_COLOR_DIAGNOSTICS ON)

if(MSCV_IDE OR MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/Ot /arch:AVX2 /fp:fast /EHsc /Zi")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /arch:AVX2 /fp:fast /EHsc /Zi /fsanitize=address")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -march=native -mtune=native -pipe -fno-builtin -g")
    set(CMAKE_CXX_FLAGS_DEBUG "-Og -march=native -mtune=native -pipe -fno-builtin -g -fsanitize=address,leak,undefined")
endif()

find_package(TBB REQUIRED)
link_libraries(TBB::tbb)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_libraries(${OpenCV_LIBS})

add_library(Utils STATIC Utils.cpp)
add_library(FractalGenerator STATIC FractalGenerator.cpp)
link_libraries(FractalGenerator Utils)

add_executable(Mandelbrot Mandelbrot.cpp MandelbrotGenerator.cpp)
add_executable(Julia Julia.cpp JuliaGenerator.cpp)
add_executable(Cosine Cosine.cpp CosineGenerator.cpp)
add_executable(Tricorn Tricorn.cpp TricornGenerator.cpp)
