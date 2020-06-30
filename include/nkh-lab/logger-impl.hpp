#ifndef NLAB_LOGGER_IMPL_HPP
#define NLAB_LOGGER_IMPL_HPP

#include "logger-types.hpp"

#include <iostream>
#include <mutex>
#include <sstream>

#include <stdarg.h>
#include <string.h>

#ifdef __linux__
#include <sys/time.h>
#endif

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
#if defined ANDROID || defined __ANDROID__
#define LOG_OUTPUT_LOGCAT
#include "logger-logcat.hpp"

#define LOG_OUTPUT_CUSTOM(type, msg) logToLogcat<type>(msg)
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

inline const char* getFileNameFromPath(const char* fileFullPath);
inline std::string toStrFileFunctionLine(const char* fileName, const char* functionName, size_t line);
inline std::string getTimeStamp();

template <MsgType type>
constexpr bool isAllowedToLog()
{
#if defined NDEBUG
    if (type == MsgType::Debug || type == MsgType::FuncEntry || type == MsgType::FuncExit)
        return false;
    else
        return true;
#else
    return true;
#endif
};

template <MsgType type>
class Msg
{
public:
    Msg(const char* fileFullPath, const char* functionName, size_t line)
    {
        if (isAllowedToLog<type>())
            mStreamTolog << toStrFileFunctionLine(getFileNameFromPath(fileFullPath), functionName, line)
                         << " ";
    };

    // disallow copying
    Msg(const Msg&) = delete;
    Msg& operator=(const Msg&) = delete;

    // disallow moving
    Msg(Msg&&) = delete;
    Msg& operator=(Msg&&) = delete;

    ~Msg()
    {
        if (isAllowedToLog<type>())
        {
#ifdef LOG_OUTPUT_COUT
            {
                std::lock_guard<std::mutex> lock(gCoutMutex);
                std::cout << getTimeStamp() << " " << getMsgTypeName<type>() << " "
                          << mStreamTolog.str() << std::endl;
            }
#endif

            LOG_OUTPUT_CUSTOM(type, mStreamTolog.str());
        }
    };

    template <typename T>
    Msg& operator<<(const T& p)
    {
        if (isAllowedToLog<type>()) mStreamTolog << p;

        return *this;
    }

private:
    std::stringstream mStreamTolog;
};

class FunctionEnterExit
{
public:
    FunctionEnterExit(const char* fileFullPath, const char* functionName, size_t line)
    {
        if (isAllowedToLog<MsgType::FuncEntry>())
        {
            mStreamTolog << toStrFileFunctionLine(
                getFileNameFromPath(fileFullPath), functionName, line);

#ifdef LOG_FUNCTION_ENTER_EXIT_THREAD_ID
            mStreamTolog << " ThreadId: " << std::this_thread::get_id();
#endif

#ifdef LOG_OUTPUT_COUT
            {
                std::lock_guard<std::mutex> lock(gCoutMutex);
                std::cout << getTimeStamp() << " " << getMsgTypeName<MsgType::FuncEntry>() << " "
                          << mStreamTolog.str() << std::endl;
            }
#endif

            LOG_OUTPUT_CUSTOM(MsgType::FuncEntry, mStreamTolog.str());
        }
    };

    // disallow copying
    FunctionEnterExit(const FunctionEnterExit&) = delete;
    FunctionEnterExit& operator=(const FunctionEnterExit&) = delete;

    // disallow moving
    FunctionEnterExit(FunctionEnterExit&&) = delete;
    FunctionEnterExit& operator=(FunctionEnterExit&&) = delete;

    ~FunctionEnterExit()
    {
        if (isAllowedToLog<MsgType::FuncExit>())
        {
#ifdef LOG_OUTPUT_COUT
            {
                std::lock_guard<std::mutex> lock(gCoutMutex);
                std::cout << getTimeStamp() << " " << getMsgTypeName<MsgType::FuncExit>() << " "
                          << mStreamTolog.str() << std::endl;
            }
#endif

            LOG_OUTPUT_CUSTOM(MsgType::FuncExit, mStreamTolog.str());
        }
    };

private:
    std::stringstream mStreamTolog;
};

/* To log format string messages
 *
 * Example:
 * logger::logMsg<logger::msgType::Info>(__FUNCTION__, __FILE__, __LINE__, "Test %d %s %c", 888, "str", 'c' );
 */
template <MsgType type>
inline int logMsg(const char* fileFullPath, const char* functionName, size_t line, const char* fmt, ...)
{
    const int LOG_BUFFER_SIZE = 256;

    char buffer[LOG_BUFFER_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, LOG_BUFFER_SIZE - 1, fmt, args);

    Msg<type>(fileFullPath, functionName, line) << buffer;

    va_end(args);

    return 0; // possible error code, now is used in CHECK to avoid unused expression result
}

inline const char* getFileNameFromPath(const char* fileFullPath)
{
    const char* ret = strrchr(fileFullPath, '/');

    if (ret)
        ++ret;
    else
        ret = fileFullPath;

    return ret;
}

inline std::string toStrFileFunctionLine(const char* fileName, const char* functionName, size_t line)
{
    std::stringstream streamTolog;

    streamTolog << fileName << ':' << line << " " << functionName << "()";

    return streamTolog.str();
}

inline std::string getTimeStampLinux()
{
    char fmt[32];
    char buf[32];
    timeval tv;
    tm* tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);
    strftime(fmt, sizeof(fmt), "%m-%d %H:%M:%S.%%03u", tm);
    snprintf(buf, sizeof(buf), fmt, tv.tv_usec / 1000);

    return std::string(buf);
}

inline std::string getTimeStamp()
{
#ifdef __linux__
    return getTimeStampLinux();
#else
    return {};
#endif
}

} // namespace logger
} // namespace nlab

#endif // NLAB_LOGGER_IMPL_HPP
