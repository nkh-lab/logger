cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

project(nkhlablogger CXX)

option(BUILD_HEADER_LIB "Build the header only library" OFF)

include_directories("include")

file(GLOB SRC_FILES
    "src/*.cpp"
    "src/*.hpp"
)

if(BUILD_HEADER_LIB)
    add_library(${PROJECT_NAME} INTERFACE)
    target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
else()
    add_library(${PROJECT_NAME} SHARED
            ${SRC_FILES})
    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()

if(ANDROID)
    find_library(ANDROID_LOG_LIB log)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${ANDROID_LOG_LIB})
else()
    add_subdirectory(tests EXCLUDE_FROM_ALL)
endif()
