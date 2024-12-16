cmake_minimum_required(VERSION 3.19)
project("Parallel Fractal Generator using pthreads" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_COLOR_DIAGNOSTICS ON)

set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -march=native -pipe -fno-builtin -g -lpthread")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -march=native -pipe -fno-builtin -g -lpthread -fsanitize=address,leak,undefined")

set(OpenCV_DIR "/mnt/c/Users/Mihaela/build")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_libraries(${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})

find_package(Threads REQUIRED)

add_executable(julia_10pthreads_optim julia_10pthreads_optim.cpp)
add_executable(mandelbrot_10pthreads_optim mandelbrot_10pthreads_optim.cpp)
add_executable(tricorn_10pthreads_optim tricorn_10pthreads_optim.cpp)
