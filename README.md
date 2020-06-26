# logger
Utility library for source code tracing and debugging.

Could be used like header only or shared library.

## Supported build systems and OSs
- CMake: Linux, Android NDK
- Soong: AOSP

## Supported sinks
- cout
- Android Logcat

## Usage
Both usage examples (header only or shared library) can be found in tests/LoggerMainTest.

### Header only library notes
1. Define next variable in your code:
```
std::mutex nlab::logger::gCoutMutex;
```
2. Define CMake option BUILD_HEADER_LIB=ON and build, e.g:
```
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_HEADER_LIB=ON ..
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
