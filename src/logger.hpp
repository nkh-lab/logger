#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <sstream>
#include <string.h>

/* Logger Settings */
#define LOG_FUNC_ENNABLED // comment out if FUNC logging unneeded
#define LOG_FUNC_THREAD_ID_ENABLED

// Output - COUT, FILE, CUSTOM
#define LOG_OUTPUT_COUT 0
#define LOG_OUTPUT_IVI  1

// Uncomment needed
#define LOG_OUTPUT LOG_OUTPUT_COUT
//#define LOG_OUTPUT LOG_OUTPUT_IVI

namespace logger {

enum class LogType
{
    Info,
    Warning,
    Error,
    Debug,
    FuncEntry,
    FuncExit
};

class Msg
{
    LogType Type;
    std::stringstream StreamTolog;

public:
    Msg(const LogType logType, const char* function, const char* file, const int line);

    // disallow copying
    Msg(const Msg&) = delete;
    Msg& operator=(const Msg&) = delete;


    // disallow moving
    Msg(Msg&&) = delete;
    Msg& operator=(Msg&&) = delete;

    ~Msg();

    template<typename T>
    Msg& operator << (const T& p)
    {
       StreamTolog << p;

       return *this;
    }
};

class Func
{
    char* Function;
    char* File;
    int   Line;

public:
    Func(const char* function, const char* file, const int line);

    // disallow copying
    Func(const Func&) = delete;
    Func& operator=(const Func&) = delete;


    // disallow moving
    Func(Func&&) = delete;
    Func& operator=(Func&&) = delete;

    ~Func();
};

/* Additional Function to Log
 *
 * They could be used in MACROS which use ternary operator
 * e.g.:
 * #define CHECK(value) (value ? true :  (logger::logMsg(logger::LogType::Error, __FUNCTION__, __FILE__, __LINE__, "Check Error!") && false))
 *
 * if (CHECK(false))
 * {
 *      ...
 * }
 *
 * Common usage example:
 *
 * logger::logMsg(logger::LogType::Info, __FUNCTION__, __FILE__, __LINE__, "Test %d %s %c", 888, "str", 'c' );
 *
 */

int logMsg(logger::LogType type, const char* func, const char* file, int line, const char* fmt, ...);

}

/* Main Macros to Log*/
#define LOG_INFO       logger::Msg(logger::LogType::Info,    __FUNCTION__, __FILE__, __LINE__)
#define LOG_WARNING    logger::Msg(logger::LogType::Warning, __FUNCTION__, __FILE__, __LINE__)
#define LOG_ERROR      logger::Msg(logger::LogType::Error,   __FUNCTION__, __FILE__, __LINE__)
#define LOG_DEBUG      logger::Msg(logger::LogType::Debug,   __FUNCTION__, __FILE__, __LINE__)

#ifdef LOG_FUNC_ENNABLED
#define LOG_FUNC logger::Func lf(__FUNCTION__, __FILE__, __LINE__)
#else
#define LOG_FUNC
#endif

#endif
