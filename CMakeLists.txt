cmake_minimum_required(VERSION 3.5)
project("test")

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(threads_and_futures threads_and_futures.cpp)
target_link_libraries(threads_and_futures rabbitmq Threads::Threads)
#add_executable(times times.cpp)

