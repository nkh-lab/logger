#ifndef NLAB_LOGGER_HPP
#define NLAB_LOGGER_HPP

#include "logger-msgtype.hpp"

#include <iostream>
#include <mutex>
#include <sstream>
#include <string.h>
#include <stdarg.h>

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
#include "logger-logcat.hpp"

#define LOG_OUTPUT_CUSTOM(type, msg) logToLogcat(type, msg)
#endif
#endif

#ifdef LOG_OUTPUT_IVI
#include "logger-ivi.hpp"

#define LOG_OUTPUT_CUSTOM(type, msg) logToIvi(type, msg)
#endif

#ifndef LOG_OUTPUT_CUSTOM
#define LOG_OUTPUT_CUSTOM(type, msg)
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

#ifdef LOG_FUNCTION_ENTER_EXIT_THREAD_ID
#include <thread>
#endif

namespace nlab {
namespace logger {

static std::mutex gCoutMutex;

inline const char* getMsgTypeName(MsgType msgType);
inline const char* getFileNameFromPath(const char* fileFullPath);
inline std::string toStrFileFunctionLine(const char* fileName, const char* functionName, size_t line);

class Msg
{
public:
    Msg(MsgType type, const char* fileFullPath, const char* functionName, size_t line) :
        mType(type)
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

            LOG_OUTPUT_CUSTOM(mType, mStreamTolog.str());
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
        if (mType == MsgType::Debug) return false;
    #endif
        return true;
    }

private:
    MsgType mType;
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
            std::cout << getMsgTypeName(MsgType::FuncEntry) << " " << mStreamTolog.str() << std::endl;
        }
    #endif

        LOG_OUTPUT_CUSTOM(MsgType::FuncEntry, mStreamTolog.str());
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
            std::cout << getMsgTypeName(MsgType::FuncExit) << " " << mStreamTolog.str() << std::endl;
        }
    #endif

        LOG_OUTPUT_CUSTOM(MsgType::FuncExit, mStreamTolog.str());
    }

private:
    std::stringstream mStreamTolog;
};
#endif

/* To log format string messages
 *
 * Example:
 * logger::logMsg(logger::msgType::Info, __FUNCTION__, __FILE__, __LINE__, "Test %d %s %c", 888, "str", 'c' );
 */
inline int logMsg(logger::MsgType type, const char* fileFullPath, const char* functionName, size_t line, const char* fmt, ...)
{
    const int LOG_BUFFER_SIZE = 256;

    char buffer[LOG_BUFFER_SIZE];
    va_list args;
    va_start (args, fmt);
    vsnprintf (buffer, LOG_BUFFER_SIZE - 1, fmt, args);

    Msg(type, functionName, getFileNameFromPath(fileFullPath), line) << buffer;

    va_end (args);

    return 0; // possible error code, now is used in CHECK to avoid unused expression result
}

inline const char* getMsgTypeName(MsgType type)
{
    const char* ret;

    switch (type)
    {
        default:
        case MsgType::Debug:
            ret = "[debug]";
            break;

        case MsgType::Error:
            ret = "[error]";
            break;

        case MsgType::Info:
            ret = "[info] ";
            break;

        case MsgType::Warning:
            ret = "[warn] ";
            break;

        case MsgType::FuncEntry:
            ret = "[--->] ";
            break;

        case MsgType::FuncExit:
            ret = "[<---] ";
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

    streamTolog << fileName << ':' << line << " " << functionName << "()";

    return streamTolog.str();
}

} // namespace logger
} // namespace nlab

/*
 * Main macros to log
 */
#define LOG_INF nlab::logger::Msg(nlab::logger::MsgType::Info,    __FILE__, __FUNCTION__, __LINE__)
#define LOG_WRN nlab::logger::Msg(nlab::logger::MsgType::Warning, __FILE__, __FUNCTION__, __LINE__)
#define LOG_ERR nlab::logger::Msg(nlab::logger::MsgType::Error,   __FILE__, __FUNCTION__, __LINE__)
#define LOG_DBG nlab::logger::Msg(nlab::logger::MsgType::Debug,   __FILE__, __FUNCTION__, __LINE__)

#ifdef LOG_FUNCTION_ENTER_EXIT
#define LOG_FNC nlab::logger::FunctionEnterExit lf(__FILE__, __FUNCTION__, __LINE__)
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
#define CHECK(value, ...) (value ? true : (nlab::logger::logMsg(nlab::logger::MsgType::Error, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__) && false))

#endif // NLAB_LOGGER_HPP
