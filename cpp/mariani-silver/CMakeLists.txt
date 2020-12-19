cmake_minimum_required(VERSION 3.17)
project(mariani_silver)

set(CMAKE_CXX_STANDARD 20)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(
        mariani_silver
        main.cpp
        benchmark_utilities.cpp
        benchmark_utilities.h
        Stopwatch.h
        Complex.h
        Complex.cpp
        Image.h
        MandelbrotPixelCalculator.h
        ImageDimensions.h
        ImageDimensions.cpp
        ImageGenerator.h
        Image.cpp
        Rectangle.h
        Rectangle.cpp
        RectangleSplitter.h
        ImageGenerationWorker.h
)

target_link_libraries(mariani_silver PRIVATE Threads::Threads)
