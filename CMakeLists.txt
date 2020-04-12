cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

project(logger CXX)

find_package (Threads)

include_directories("include")

file(GLOB SRC_FILES
    "src/*.cpp"
    "src/*.hpp"
)

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})

target_link_libraries (${CMAKE_PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

