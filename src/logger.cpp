#include "logger.hpp"
#ifdef LOG_FUNC_THREAD_ID_ENABLED
#include <thread>
#endif
#if LOG_OUTPUT == LOG_OUTPUT_IVI
#include "ivi-logging-config.hpp"
#endif

namespace logger {

const char* getMsgTypeName(const LogType msgType);
const char* getFileNameFromPath(const char* path);

#if LOG_OUTPUT == LOG_OUTPUT_IVI
void logToIvi(const LogType logType, std::string textToLog);
#endif

Msg::Msg(const LogType logType, const char* function, const char* file, const int line)
{
    Type = logType;

#if LOG_OUTPUT == LOG_OUTPUT_COUT
    StreamTolog << getMsgTypeName(logType) << " ";
#endif

    StreamTolog << " " << getFileNameFromPath(file) << " " << function << "() line " << line << ": ";
}

Msg::~Msg()
{
    StreamTolog << "\n";

#if LOG_OUTPUT == LOG_OUTPUT_COUT
    std::cout << StreamTolog.str();
#elif LOG_OUTPUT == LOG_OUTPUT_IVI
    logToIvi(Type, StreamTolog.str());
#endif

}

Func::Func(const char* function, const char* file, const int line)
{
    Function = (char*)function;
    File = (char*)getFileNameFromPath(file);
    Line = line;

    std::stringstream streamTolog;

#if LOG_OUTPUT == LOG_OUTPUT_COUT || LOG_OUTPUT == LOG_OUTPUT_IVI
    streamTolog << getMsgTypeName(LogType::FuncEntry) << " ";
#endif

    streamTolog << " " << File << " " << Function << "() line " << Line << ":";

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

Func::~Func()
{
    std::stringstream streamTolog;

#if LOG_OUTPUT == LOG_OUTPUT_COUT || LOG_OUTPUT == LOG_OUTPUT_IVI
    streamTolog << getMsgTypeName(LogType::FuncExit) << " ";
#endif

    streamTolog << " " << File << " " << Function << "() line " << Line << ":";

#ifdef LOG_FUNC_THREAD_ID_ENABLED
    streamTolog << " ThreadId = " << std::this_thread::get_id();
#endif

    streamTolog << "\n";

#if LOG_OUTPUT == LOG_OUTPUT_COUT
    std::cout << streamTolog.str();
#elif LOG_OUTPUT == LOG_OUTPUT_IVI
    logToIvi(LogType::FuncExit, streamTolog.str());
#endif
}

const char* getMsgTypeName(const LogType logType)
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

const char* getFileNameFromPath(const char* path)
{
   for(size_t i = strlen(path) - 1;  i >= 0; i--)
   {
      if (path[i] == '/')
      {
         return (char*)&path[i+1];
      }
   }

   return (char*)path;
}

#if LOG_OUTPUT == LOG_OUTPUT_IVI
void logToIvi(const LogType logType, std::string textToLog)
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
#endif

}
