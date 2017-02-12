cmake_minimum_required(VERSION 3.5.1)

project(Deque)

find_package(Threads)
find_package(GTest)


add_compile_options(-O0)
link_libraries(pthread)
link_libraries(gtest)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES deque.h tests.h main.cpp)
add_executable(Deque ${SOURCE_FILES})

target_link_libraries(Deque pthread gtest)
