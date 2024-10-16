cmake_minimum_required(VERSION 3.19)
project("Serial Fractal Generator" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_COLOR_DIAGNOSTICS ON)

if(MSCV_IDE OR MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/Ot /arch:AVX2 /fp:fast /EHsc /Zi")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /arch:AVX2 /fp:fast /EHsc /Zi /fsanitize=address")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -march=native -pipe -fno-builtin -g")
    set(CMAKE_CXX_FLAGS_DEBUG "-Og -march=native -pipe -fno-builtin -g -fsanitize=address,leak,undefined")
endif()

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_libraries(${OpenCV_LIBS})

set(SOURCE_FILES Utils.cpp FractalGenerator.cpp)

add_executable(Mandelbrot Mandelbrot.cpp MandelbrotGenerator.cpp ${SOURCE_FILES})
add_executable(Julia Julia.cpp JuliaGenerator.cpp ${SOURCE_FILES})
add_executable(Cosine Cosine.cpp CosineGenerator.cpp ${SOURCE_FILES})
add_executable(Tricorn Tricorn.cpp TricornGenerator.cpp ${SOURCE_FILES})
