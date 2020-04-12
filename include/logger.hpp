#ifndef LOGGER_HPP
#define LOGGER_HPP

/* Logger Settings */
#define LOG_FUNC_ENNABLED // comment out if FUNC logging unneeded
#define LOG_FUNC_THREAD_ID_ENABLED

// Output - COUT, FILE, CUSTOM
#define LOG_OUTPUT_COUT 0
#define LOG_OUTPUT_IVI  1

// Uncomment needed
#define LOG_OUTPUT LOG_OUTPUT_COUT
//#define LOG_OUTPUT LOG_OUTPUT_IVI

#include <iostream>
#include <sstream>
#include <string.h>
#include <stdarg.h>

#ifdef LOG_FUNC_THREAD_ID_ENABLED
#include <thread>
#endif
#if LOG_OUTPUT == LOG_OUTPUT_IVI
#include "ivi-logging-config.hpp"
#endif

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

inline const char* getMsgTypeName(const LogType logType);
inline const char* getFileNameFromPath(const char* fileFullPath);

class Msg
{
public:
    Msg(LogType logType, const char* functionName, const char* fileFullPath, size_t line)
    {
        mType = logType;

        mStreamTolog << " " << getFileNameFromPath(fileFullPath) << " " << functionName << "() line " << line << ": ";
    }

    // disallow copying
    Msg(const Msg&) = delete;
    Msg& operator=(const Msg&) = delete;


    // disallow moving
    Msg(Msg&&) = delete;
    Msg& operator=(Msg&&) = delete;

    ~Msg()
    {
        mStreamTolog << "\n";

    #if LOG_OUTPUT == LOG_OUTPUT_COUT
        std::cout << getMsgTypeName(mType) << " " << mStreamTolog.str();
    #elif LOG_OUTPUT == LOG_OUTPUT_IVI
        logToIvi(mType, StreamTolog.str());
    #endif

    }

    template<typename T>
    Msg& operator << (const T& p)
    {
       mStreamTolog << p;

       return *this;
    }

private:
    LogType mType;
    std::stringstream mStreamTolog;
};

class Func
{
public:
    Func(const char* function, const char* file, size_t line)
    {
        mFunctionName = (char*)function;
        mFileName = (char*)getFileNameFromPath(file);
        mLine = line;

        std::stringstream streamTolog;

    #if LOG_OUTPUT == LOG_OUTPUT_COUT || LOG_OUTPUT == LOG_OUTPUT_IVI
        streamTolog << getMsgTypeName(LogType::FuncEntry) << " ";
    #endif

        streamTolog << " " << mFileName << " " << mFunctionName << "() line " << mLine << ":";

    #ifdef LOG_FUNC_THREAD_ID_ENABLED
        streamTolog << " ThreadId = " << std::this_thread::get_id();
    #endif

       streamTolog << "\n";

    #if LOG_OUTPUT == LOG_OUTPUT_COUT
        std::cout << streamTolog.str();
    #elif LOG_OUTPUT == LOG_OUTPUT_IVI
        logToIvi(LogType::FuncEntry, streamTolog.str());
    #endif
    }

    // disallow copying
    Func(const Func&) = delete;
    Func& operator=(const Func&) = delete;

    // disallow moving
    Func(Func&&) = delete;
    Func& operator=(Func&&) = delete;

    ~Func()
    {
        std::stringstream streamTolog;

    #if LOG_OUTPUT == LOG_OUTPUT_COUT || LOG_OUTPUT == LOG_OUTPUT_IVI
        streamTolog << getMsgTypeName(LogType::FuncExit) << " ";
    #endif

        streamTolog << " " << mFileName << " " << mFunctionName << "() line " << mLine << ":";

    #ifdef LOG_FUNC_THREAD_ID_ENABLED
        streamTolog << " ThreadId = " << std::this_thread::get_id();
    #endif

        streamTolog << std::endl;

    #if LOG_OUTPUT == LOG_OUTPUT_COUT
        std::cout << streamTolog.str();
    #elif LOG_OUTPUT == LOG_OUTPUT_IVI
        logToIvi(LogType::FuncExit, streamTolog.str());
    #endif
    }

private:
    char* mFunctionName;
    char* mFileName;
    size_t mLine;
};

/* To log format string messages
 *
 * Example:
 * logger::logMsg(logger::LogType::Info, __FUNCTION__, __FILE__, __LINE__, "Test %d %s %c", 888, "str", 'c' );
 */
inline void logMsg(logger::LogType type, const char* func, const char* file, size_t line, const char* fmt, ...)
{
    const int LOG_BUFFER_SIZE = 256;

    char buffer[LOG_BUFFER_SIZE];
    va_list args;
    va_start (args, fmt);
    vsnprintf (buffer, LOG_BUFFER_SIZE - 1, fmt, args);

    Msg(type, func, getFileNameFromPath(file), line) << buffer;

    va_end (args);
}

inline const char* getMsgTypeName(const LogType logType)
{
    const char* ret;

    switch (logType)
    {
        default:
        case LogType::Debug:
            ret = "LOG_DEBUG     ";
            break;

        case LogType::Error:
            ret = "LOG_ERROR     ";
            break;

        case LogType::Info:
            ret = "LOG_INFO      ";
            break;

        case LogType::Warning:
            ret = "LOG_WARNING   ";
            break;

        case LogType::FuncEntry:
            ret = "LOG_FUNC_ENTRY";
            break;

        case LogType::FuncExit:
            ret = "LOG_FUNC_EXIT ";
            break;
    }

    return ret;
}

inline const char* getFileNameFromPath(const char* fileFullPath)
{
   for(size_t i = strlen(fileFullPath) - 1;  i >= 0; i--)
   {
      if (fileFullPath[i] == '/')
      {
         return (char*)&fileFullPath[i+1];
      }
   }

   return (char*)fileFullPath;
}

#if LOG_OUTPUT == LOG_OUTPUT_IVI
void logToIvi(const LogType logType, std::string textToLog)
{
    try
    {
        switch (logType)
        {
            case LogType::Debug:
            case LogType::FuncEntry:
            case LogType::FuncExit:
                log_debug() << textToLog;
                break;

            case LogType::Error:
                log_error() << textToLog;
                break;

            case LogType::Info:
                log_info() << textToLog;
                break;

            case LogType::Warning:
                log_warning() << textToLog;
                break;

            default:
                break;
        }
    }
    catch(...){}
}
#endif

}

/* Main Macros to Log */
#define LOG_INFO       logger::Msg(logger::LogType::Info,    __FUNCTION__, __FILE__, __LINE__)
#define LOG_WARNING    logger::Msg(logger::LogType::Warning, __FUNCTION__, __FILE__, __LINE__)
#define LOG_ERROR      logger::Msg(logger::LogType::Error,   __FUNCTION__, __FILE__, __LINE__)
#define LOG_DEBUG      logger::Msg(logger::LogType::Debug,   __FUNCTION__, __FILE__, __LINE__)

#ifdef LOG_FUNC_ENNABLED
#define LOG_FUNC logger::Func lf(__FUNCTION__, __FILE__, __LINE__)
#else
#define LOG_FUNC
#endif

/* Check and log if false
 *
 * Example:
 * if (CHECK(ptr, "ptr is NULL"))
 * {
 *      ...
 * }
 */
#define CHECK(value, ...) (value ? true : (logger::logMsg(logger::LogType::Error, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__), false))

#endif
