# logger
Utility library for source code tracing and debugging.

Could be used like interface (header only) or shared library.

## Supported build systems
- CMake
- Soong AOSP

## Supported sinks
- `std::cout` - default
- Android Logcat - if Android (AOSP and NDK)

## Usage
Both usage examples (header only or shared library) can be found in tests/LoggerMainTest.

### Interface (header only) library notes
1. Define next variable in your code if it uses `std::cout` as logging output:
```
std::mutex nlab::logger::gCoutMutex; // used for output synchronization
```
If not, switch off cout logging via next define: `LOG_OUTPUT_COUT_DISABLED`, e.g. for CMake:
```
add_definitions(-DLOG_OUTPUT_COUT_DISABLED`)
```

2. For CMake build set the option `LOGGER_BUILD_HEADER_LIB=ON` via command line, e.g:
```
cmake -DCMAKE_BUILD_TYPE=Release -DLOGGER_BUILD_HEADER_LIB=ON ..
```
or in the top CMake file, e.g:
```
set(LOGGER_BUILD_HEADER_LIB ON CACHE BOOL "Build the header only library")
add_subdirectory(logger)
```

## Example outputs
Outputs examples captured from tests/LoggerMainTest

For Debug build:
```
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cd tests
make
./LoggerMainTest
```
```
06-26 17:23:21.897 [--->]  LoggerMainTest.cpp:16 main() ThreadId: 140718535841600
06-26 17:23:21.897 [--->]  LoggerMainTest.cpp:60 main_th() ThreadId: 140718515595008
06-26 17:23:21.897 [--->]  LoggerMainTest.cpp:60 main_th() ThreadId: 140718507202304
06-26 17:23:21.897 [--->]  LoggerMainTest.cpp:55 test2() ThreadId: 140718507202304
06-26 17:23:21.897 [--->]  LoggerMainTest.cpp:55 test2() ThreadId: 140718515595008
06-26 17:23:21.897 [<---]  LoggerMainTest.cpp:55 test2() ThreadId: 140718515595008
06-26 17:23:21.898 [<---]  LoggerMainTest.cpp:55 test2() ThreadId: 140718507202304
06-26 17:23:21.998 [--->]  LoggerMainTest.cpp:55 test2() ThreadId: 140718515595008
06-26 17:23:21.998 [<---]  LoggerMainTest.cpp:55 test2() ThreadId: 140718515595008
06-26 17:23:21.998 [--->]  LoggerMainTest.cpp:55 test2() ThreadId: 140718507202304
06-26 17:23:21.998 [<---]  LoggerMainTest.cpp:55 test2() ThreadId: 140718507202304
06-26 17:23:22.098 [--->]  LoggerMainTest.cpp:55 test2() ThreadId: 140718515595008
06-26 17:23:22.098 [<---]  LoggerMainTest.cpp:55 test2() ThreadId: 140718515595008
06-26 17:23:22.098 [--->]  LoggerMainTest.cpp:55 test2() ThreadId: 140718507202304
06-26 17:23:22.098 [<---]  LoggerMainTest.cpp:55 test2() ThreadId: 140718507202304
06-26 17:23:22.198 [<---]  LoggerMainTest.cpp:60 main_th() ThreadId: 140718515595008
06-26 17:23:22.198 [<---]  LoggerMainTest.cpp:60 main_th() ThreadId: 140718507202304
06-26 17:23:22.198 [info]  LoggerMainTest.cpp:24 main() Test Info 1
06-26 17:23:22.198 [warn]  LoggerMainTest.cpp:25 main() Test Warning 2
06-26 17:23:22.198 [error] LoggerMainTest.cpp:26 main() Test Error 3
06-26 17:23:22.198 [debug] LoggerMainTest.cpp:27 main() Test Debug 4
06-26 17:23:22.198 [info]  LoggerMainTest.cpp:30 main() Test 888 str c
06-26 17:23:22.199 [error] LoggerMainTest.cpp:33 main() ptr = nullptr
06-26 17:23:22.199 [--->]  LoggerMainTest.cpp:50 test1() ThreadId: 140718535841600
06-26 17:23:22.199 [<---]  LoggerMainTest.cpp:50 test1() ThreadId: 140718535841600
06-26 17:23:22.199 [--->]  LoggerMainTest.cpp:50 test1() ThreadId: 140718535841600
06-26 17:23:22.199 [<---]  LoggerMainTest.cpp:50 test1() ThreadId: 140718535841600
06-26 17:23:22.199 [--->]  LoggerMainTest.cpp:50 test1() ThreadId: 140718535841600
06-26 17:23:22.199 [<---]  LoggerMainTest.cpp:50 test1() ThreadId: 140718535841600
06-26 17:23:23.199 [<---]  LoggerMainTest.cpp:16 main() ThreadId: 140718535841600
```
For Release build:
```
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cd tests
make
./LoggerMainTest
```
```
06-26 17:37:00.411 [info]  LoggerMainTest.cpp:24 main() Test Info 1
06-26 17:37:00.411 [warn]  LoggerMainTest.cpp:25 main() Test Warning 2
06-26 17:37:00.411 [error] LoggerMainTest.cpp:26 main() Test Error 3
06-26 17:37:00.411 [info]  LoggerMainTest.cpp:30 main() Test 888 str c
06-26 17:37:00.411 [error] LoggerMainTest.cpp:33 main() ptr = nullptr
```
