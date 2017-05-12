cmake_minimum_required(VERSION 2.8)

# FLAGS:
# -std=c++11    - c++11 support
# -Wall         - all warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11 -Wall")


# project name e.g. for qt-creator
set(PROJ_NAME "logger")
project("${PROJ_NAME}")
#message("****** PROJ_NAME: " ${PROJ_NAME})


#FIND_PACKAGE(PkgConfig)
#PKG_CHECK_MODULES(MODULE1 REQUIRED ivi-logging)

#message("****** MODULE1_LIBRARIES: " ${MODULE1_LIBRARIES})
#message("****** MODULE1_LIBRARY_DIRS: " ${MODULE1_LIBRARY_DIRS})


#include_directories(${MODULE1_INCLUDE_DIRS})

file(GLOB SRC_FILES
    "src/*.cpp"
    "src/*.hpp"
)

#message("****** SRC_FILES: " ${SRC_FILES})

#link_directories(${MODULE1_LIBRARY_DIRS})

add_executable("${PROJ_NAME}" ${SRC_FILES})

#target_link_libraries("${PROJ_NAME}" ${MODULE1_LIBRARIES})
