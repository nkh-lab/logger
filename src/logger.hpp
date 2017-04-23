#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <sstream>
#include <string.h>

/* Logger Settings */
#define LOG_FUNC_ENNABLED // comment out if FUNC logging unnneded

//Output
#define LOGGER_OUTPUT COUT // COUT, FILE, CUSTOM

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
    std::stringstream StreamTolog;

public:
    Msg(const LogType logType, const char* function, const char* file, const int line);

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

public:
    Func(const LogType logType, const char* function, const char* file, const int line);
};

}

// Macros to log messages
#define LOG_INFO       logger::Msg(logger::LogType::Info,    __FUNCTION__, __FILE__, __LINE__)
#define LOG_WARNING    logger::Msg(logger::LogType::Warning, __FUNCTION__, __FILE__, __LINE__)
#define LOG_ERROR      logger::Msg(logger::LogType::Error,   __FUNCTION__, __FILE__, __LINE__)
#define LOG_DEBUG      logger::Msg(logger::LogType::Debug,   __FUNCTION__, __FILE__, __LINE__)

#ifdef LOG_FUNC_ENNABLED

#define LOG_FUNC_ENTRY logger::Func(logger::LogType::FuncEntry, __FUNCTION__, __FILE__, __LINE__)
#define LOG_FUNC_EXIT  logger::Func(logger::LogType::FuncExit,  __FUNCTION__, __FILE__, __LINE__)

#else

#define LOG_FUNC_ENTRY
#define LOG_FUNC_EXIT


#endif


#endif
