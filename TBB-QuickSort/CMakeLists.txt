cmake_minimum_required(VERSION 3.19)
project("QuickSort using TBB" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_COLOR_DIAGNOSTICS ON)

if(MSCV_IDE OR MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/Ot /arch:AVX2 /fp:fast /EHsc /Zi")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /arch:AVX2 /fp:fast /EHsc /Zi /fsanitize=address")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -march=native -pipe -fno-builtin -g")
    set(CMAKE_CXX_FLAGS_DEBUG "-Og -march=native -pipe -fno-builtin -g -fsanitize=address,leak,undefined")
endif()

add_executable(QuickSort main.cpp Utils.cpp QuickSort.hpp)

find_package(TBB REQUIRED)
target_link_libraries(QuickSort PRIVATE TBB::tbb)
