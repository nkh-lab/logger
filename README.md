# logger
To trace and debug a source code.
Could be used like header only or shared library.

# Supported build systems and OSs
- CMake: Linux, Android NDK
- Soong: AOSP

# Header only library
In this case next variable should be defined:
```
std::mutex nlab::logger::gCoutMutex;
```
