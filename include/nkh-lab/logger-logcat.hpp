#ifndef NLAB_LOGGER_LOGCAT_HPP
#define NLAB_LOGGER_LOGCAT_HPP

#include "android/log.h"
#include "logger-types.hpp"

#include <string>

#ifndef LOG_TAG
#define LOG_TAG NULL
#endif

namespace nlab {
namespace logger {

inline void logToLogcat(MsgType logType, std::string textToLog)
{
    switch (logType)
    {
    case MsgType::Debug:
        (void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "%s", textToLog.c_str());
        break;

    case MsgType::FuncEntry:
    case MsgType::FuncExit:
        (void)__android_log_print(
            ANDROID_LOG_DEBUG, LOG_TAG, "%s%s", getMsgTypeName(logType), textToLog.c_str());
        break;

    case MsgType::Error:
        (void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s", textToLog.c_str());
        break;

    case MsgType::Info:
        (void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%s", textToLog.c_str());
        break;

    case MsgType::Warning:
        (void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, "%s", textToLog.c_str());
        break;

    default:
        break;
    }
}

} // namespace logger
} // namespace nlab

#endif // NLAB_LOGGER_LOGCAT_HPP
