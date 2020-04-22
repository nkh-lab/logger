#ifndef NLAB_LOGGER_MSGTYPE_HPP
#define NLAB_LOGGER_MSGTYPE_HPP


#include <iostream>
#include <mutex>
#include <sstream>

#include <stdarg.h>
#include <string.h>

/*
 * Outputs: COUT or CUSTOM: Android Logcat, FILE, ...
 *
 *  - LOG_OUTPUT_COUT   - ON  by default
 *  - LOG_OUTPUT_LOGCAT - ON  by default for ANDROID build
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

#ifndef LOG_OUTPUT_CUSTOM
#define LOG_OUTPUT_CUSTOM(type, msg)
#endif

#ifdef LOG_FUNCTION_ENTER_EXIT_THREAD_ID
#include <thread>
#endif

namespace nlab {
namespace logger {

extern std::mutex gCoutMutex;

enum class MsgType
{
    Info,
    Warning,
    Error,
    Debug,
    FuncEntry,
    FuncExit
};

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
    };

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
    };

    template<typename T>
    Msg& operator << (const T& p)
    {
        if (isAllowedToLog()) mStreamTolog << p;

        return *this;
    }

private:
    bool isAllowedToLog()
    {
    #ifdef LOG_DEBUG_MESSAGES
        if (mType == MsgType::Debug) return false;
    #endif
        return true;
    };

private:
    MsgType mType;
    std::stringstream mStreamTolog;
};

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
    };

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
    };

private:
    std::stringstream mStreamTolog;
};

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
    const char* ret = strrchr(fileFullPath, '/');

    if (ret) ++ret;

    return ret;
}

inline std::string toStrFileFunctionLine(const char* fileName, const char* functionName, size_t line)
{
    std::stringstream streamTolog;

    streamTolog << fileName << ':' << line << " " << functionName << "()";

    return streamTolog.str();
}

} // namespace logger
} // namespace nlab

#endif // NLAB_LOGGER_MSGTYPE_HPP
