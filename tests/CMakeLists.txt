cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

add_definitions(-DLOG_FUNCTION_ENTER_EXIT)
add_definitions(-DLOG_FUNCTION_ENTER_EXIT_THREAD_ID)

project(MainLoggerTest CXX)

find_package (Threads)

include_directories("../include")

file(GLOB SRC_FILES
    "MainLoggerTest.cpp"
)

include_directories(../include)

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})

target_link_libraries (${CMAKE_PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/../build/libnkhlablogger.so
    ${CMAKE_THREAD_LIBS_INIT})
