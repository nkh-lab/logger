#include "nkh-lab/logger.hpp"

#include <iostream>
#include <mutex>

#include <stdarg.h>

/*
 * See logging macros at the end of the file
 */

/* Logger Settings */

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

std::mutex gCoutMutex;

const char* getMsgTypeName(MsgType msgType);
const char* getFileNameFromPath(const char* fileFullPath);
std::string toStrFileFunctionLine(const char* fileName, const char* functionName, size_t line);


Msg::Msg(MsgType type, const char* fileFullPath, const char* functionName, size_t line) :
    mType(type)
{
    if (isAllowedToLog())
        mStreamTolog << toStrFileFunctionLine(getFileNameFromPath(fileFullPath), functionName, line) << " ";
};

Msg::~Msg()
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

bool Msg::isAllowedToLog()
{
#ifdef LOG_DEBUG_MESSAGES
    if (mType == MsgType::Debug) return false;
#endif
    return true;
};

FunctionEnterExit::FunctionEnterExit(const char* fileFullPath, const char* functionName, size_t line)
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


FunctionEnterExit::~FunctionEnterExit()
{
#ifdef LOG_OUTPUT_COUT
    {
        std::lock_guard<std::mutex> lock(gCoutMutex);
        std::cout << getMsgTypeName(MsgType::FuncExit) << " " << mStreamTolog.str() << std::endl;
    }
#endif

    LOG_OUTPUT_CUSTOM(MsgType::FuncExit, mStreamTolog.str());
};

/* To log format string messages
 *
 * Example:
 * logger::logMsg(logger::msgType::Info, __FUNCTION__, __FILE__, __LINE__, "Test %d %s %c", 888, "str", 'c' );
 */
int logMsg(logger::MsgType type, const char* fileFullPath, const char* functionName, size_t line, const char* fmt, ...)
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

const char* getMsgTypeName(MsgType type)
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

const char* getFileNameFromPath(const char* fileFullPath)
{
   for(size_t i = strlen(fileFullPath) - 1;  i != 0; i--)
   {
      if (fileFullPath[i] == '/')
      {
         return (char*)&fileFullPath[i+1];
      }
   }

   return (char*)fileFullPath;
}

std::string toStrFileFunctionLine(const char* fileName, const char* functionName, size_t line)
{
    std::stringstream streamTolog;

    streamTolog << fileName << ':' << line << " " << functionName << "()";

    return streamTolog.str();
}

} // namespace logger
} // namespace nlab
