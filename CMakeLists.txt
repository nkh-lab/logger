cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

project(nkhlablogger CXX)

option(LOGGER_BUILD_HEADER_LIB "Build the header only library" OFF)

if(ANDROID)
    find_library(ANDROID_LOG_LIB log)
endif()

if(LOGGER_BUILD_HEADER_LIB)
    add_library(${PROJECT_NAME} INTERFACE)
    target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

    if(ANDROID)
        target_link_libraries(${PROJECT_NAME} INTERFACE ${ANDROID_LOG_LIB})
    endif()
else()
    add_library(${PROJECT_NAME} SHARED src/logger.cpp)
    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

    if(ANDROID)
        target_link_libraries(${PROJECT_NAME} PUBLIC ${ANDROID_LOG_LIB})
    endif()
endif()

if(NOT ANDROID)
    add_subdirectory(tests EXCLUDE_FROM_ALL)
endif()
