cmake_minimum_required(VERSION 2.8)

# FLAGS:
# -std=c++11    - c++11 support
# -Wall         - all warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11 -Wall")

# project name e.g. for qt-creator
project(logger)

file(GLOB srcFiles
    "src/*.cpp"
    "src/*.hpp"

)

# show in cmake log
message("Detected source files: " ${srcFiles})

add_executable(cpp ${srcFiles})

