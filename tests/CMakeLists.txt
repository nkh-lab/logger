cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

add_definitions(-DLOG_FUNCTION_ENTER_EXIT)
add_definitions(-DLOG_FUNCTION_ENTER_EXIT_THREAD_ID)

if(BUILD_HEADER_LIB)
    add_definitions(-DBUILD_HEADER_LIB)
endif()

project(LoggerMainTest CXX)

find_package (Threads)

file(GLOB SRC_FILES
    "LoggerMainTest.cpp"
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries (${PROJECT_NAME}
    nkhlablogger
    ${CMAKE_THREAD_LIBS_INIT})
