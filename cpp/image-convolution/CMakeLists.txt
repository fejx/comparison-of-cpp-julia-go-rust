cmake_minimum_required(VERSION 3.16)
project(image_convolution)

set(CMAKE_CXX_STANDARD 20)

find_package(Threads REQUIRED)

add_executable(image_convolution main.cpp Image.h Kernel.h Kernel.cpp convolution.h convolution.cpp Stopwatch.h benchmark_utilities.h benchmark_utilities.cpp)

target_link_libraries(image_convolution
        ${CMAKE_THREAD_LIBS_INIT}
        )
