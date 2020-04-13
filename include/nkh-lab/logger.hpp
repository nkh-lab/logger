#ifndef NKH_LAB_LOGGER_HPP
#define NKH_LAB_LOGGER_HPP

/*
 * See logging macros at the end of the file
 */

/* Logger Settings */

/*
 * Outputs: COUT, Android Logcat, FILE, CUSTOM
 *
 *  - LOG_OUTPUT_COUT   - ON  by default
 *  - LOG_OUTPUT_LOGCAT - ON  by default for ANDROID build
 *  - LOG_OUTPUT_IVI    - OFF by default
 */
#ifndef LOG_OUTPUT_COUT_DISABLED
#define LOG_OUTPUT_COUT
#endif

#ifndef LOG_OUTPUT_LOGCAT_DISABLED
#ifdef ANDROID
#define LOG_OUTPUT_LOGCAT
#endif
#endif

/*
 * Function Enter-Exit logging
 *
 * Swithed OFF by default
 *  - LOG_FUNCTION_ENTER_EXIT           - OFF
 *  - LOG_FUNCTION_ENTER_EXIT_THREAD_ID - OFF
 */

#ifdef LOG_FUNCTION_ENTER_EXIT
#ifdef NDEBUG
#undef LOG_FUNCTION_ENTER_EXIT
#endif
#endif

#include <iostream>
#include <mutex>
#include <sstream>
#include <string.h>
#include <stdarg.h>

#ifdef LOG_FUNCTION_ENTER_EXIT_THREAD_ID
#include <thread>
#endif
#ifdef LOG_OUTPUT_IVI
#include "ivi-logging-config.hpp"
#endif

namespace logger {

std::mutex gCoutMutex;

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
inline std::string toStrFileFunctionLine(const char* fileName, const char* functionName, size_t line);

class Msg
{
public:
    Msg(LogType logType, const char* fileFullPath, const char* functionName, size_t line) :
        mType(logType)
    {
        if (isAllowedToLog())
            mStreamTolog << toStrFileFunctionLine(getFileNameFromPath(fileFullPath), functionName, line) << " ";
    }

    // disallow copying
    Msg(const Msg&) = delete;
    Msg& operator=(const Msg&) = delete;


    // disallow moving
    Msg(Msg&&) = delete;
    Msg& operator=(Msg&&) = delete;

    ~Msg()
    {
        if (isAllowedToLog())
        {
        #ifdef LOG_OUTPUT_COUT
            {
                std::lock_guard<std::mutex> lock(gCoutMutex);
                std::cout << getMsgTypeName(mType) << " " << mStreamTolog.str() << std::endl;
            }
        #endif

        #ifdef LOG_OUTPUT_IVI
            logToIvi(mType, StreamTolog.str());
        #endif
        }
    }

    template<typename T>
    Msg& operator << (const T& p)
    {
        if (isAllowedToLog()) mStreamTolog << p;

        return *this;
    }

private:
    inline bool isAllowedToLog()
    {
    #ifdef NDEBUG
        if (mType == LogType::Debug) return false;
    #endif
        return true;
    }

private:
    LogType mType;
    std::stringstream mStreamTolog;
};

#ifdef LOG_FUNCTION_ENTER_EXIT
class FunctionEnterExit
{
public:
    FunctionEnterExit(const char* fileFullPath, const char* functionName, size_t line)
    {
        mStreamTolog << toStrFileFunctionLine(getFileNameFromPath(fileFullPath), functionName, line);

    #ifdef LOG_FUNCTION_ENTER_EXIT_THREAD_ID
        mStreamTolog << " ThreadId: " << std::this_thread::get_id();
    #endif

    #ifdef LOG_OUTPUT_COUT
        {
            std::lock_guard<std::mutex> lock(gCoutMutex);
            std::cout << getMsgTypeName(LogType::FuncEntry) << " " << mStreamTolog.str() << std::endl;
        }
    #endif

    #ifdef LOG_OUTPUT_IVI
        logToIvi(LogType::FuncEntry, mStreamTolog.str());
    #endif
    }

    // disallow copying
    FunctionEnterExit(const FunctionEnterExit&) = delete;
    FunctionEnterExit& operator=(const FunctionEnterExit&) = delete;

    // disallow moving
    FunctionEnterExit(FunctionEnterExit&&) = delete;
    FunctionEnterExit& operator=(FunctionEnterExit&&) = delete;

    ~FunctionEnterExit()
    {
    #ifdef LOG_OUTPUT_COUT
        {
            std::lock_guard<std::mutex> lock(gCoutMutex);
            std::cout << getMsgTypeName(LogType::FuncExit) << " " << mStreamTolog.str() << std::endl;
        }
    #endif

    #ifdef LOG_OUTPUT_IVI
        logToIvi(LogType::FuncEntry, mStreamTolog.str());
    #endif
    }

private:
    std::stringstream mStreamTolog;
};
#endif
/* To log format string messages
 *
 * Example:
 * logger::logMsg(logger::LogType::Info, __FUNCTION__, __FILE__, __LINE__, "Test %d %s %c", 888, "str", 'c' );
 */
inline void logMsg(logger::LogType type, const char* fileFullPath, const char* functionName, size_t line, const char* fmt, ...)
{
    const int LOG_BUFFER_SIZE = 256;

    char buffer[LOG_BUFFER_SIZE];
    va_list args;
    va_start (args, fmt);
    vsnprintf (buffer, LOG_BUFFER_SIZE - 1, fmt, args);

    Msg(type, functionName, getFileNameFromPath(fileFullPath), line) << buffer;

    va_end (args);
}

inline const char* getMsgTypeName(const LogType logType)
{
    const char* ret;

    switch (logType)
    {
        default:
        case LogType::Debug:
            ret = "LOG_DBG";
            break;

        case LogType::Error:
            ret = "LOG_ERR";
            break;

        case LogType::Info:
            ret = "LOG_INF";
            break;

        case LogType::Warning:
            ret = "LOG_WRN";
            break;

        case LogType::FuncEntry:
            ret = "LOG_FEN";
            break;

        case LogType::FuncExit:
            ret = "LOG_FEX";
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

inline std::string toStrFileFunctionLine(const char* fileName, const char* functionName, size_t line)
{
    std::stringstream streamTolog;

    streamTolog << fileName << ':' << functionName << "():" << line << ':';

    return streamTolog.str();
}

#ifdef LOG_OUTPUT_IVI
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

/*
 * Main macros to log
 */
#define LOG_INF logger::Msg(logger::LogType::Info,    __FILE__, __FUNCTION__, __LINE__)
#define LOG_WRN logger::Msg(logger::LogType::Warning, __FILE__, __FUNCTION__, __LINE__)
#define LOG_ERR logger::Msg(logger::LogType::Error,   __FILE__, __FUNCTION__, __LINE__)
#define LOG_DBG logger::Msg(logger::LogType::Debug,   __FILE__, __FUNCTION__, __LINE__)

#ifdef LOG_FUNCTION_ENTER_EXIT
#define LOG_FNC logger::FunctionEnterExit lf(__FILE__, __FUNCTION__, __LINE__)
#else
#define LOG_FNC
#endif

/* Check and log if false
 *
 * Example:
 * if (CHECK(ptr, "ptr is NULL"))
 * {
      ...
 * }
 */
#define CHECK(value, ...) (value ? true : (logger::logMsg(logger::LogType::Error, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__), false))

#endif // NKH_LAB_LOGGER_HPP
