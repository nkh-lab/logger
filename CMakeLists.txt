cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

add_definitions(-DLOG_FUNCTION_ENTER_EXIT_THREAD_ID)

project(nkhlablogger CXX)

find_package (Threads)

include_directories("include")

file(GLOB SRC_FILES
    "src/*.cpp"
    "src/*.hpp"
)

add_library(${PROJECT_NAME} SHARED
            ${SRC_FILES})
