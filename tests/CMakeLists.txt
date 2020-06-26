cmake_minimum_required(VERSION 2.8)

project(LoggerMainTest CXX)

add_definitions(-DLOG_FUNCTION_ENTER_EXIT_THREAD_ID)

if(BUILD_HEADER_LIB)
    add_definitions(-DBUILD_HEADER_LIB)
endif()

find_package (Threads)

add_executable(${PROJECT_NAME} "LoggerMainTest.cpp")

target_link_libraries (${PROJECT_NAME}
    nkhlablogger
    ${CMAKE_THREAD_LIBS_INIT})
