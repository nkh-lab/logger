#ifndef NLAB_LOGGER_LOGCAT_HPP
#define NLAB_LOGGER_LOGCAT_HPP

#include "logger-msgtype.hpp"
#include "log/log.h"

#include <string>

namespace nlab {
namespace logger {

inline void logToLogcat(MsgType logType, std::string textToLog)
{
    switch (logType)
    {
        case MsgType::Debug:
            ALOGD("%s", textToLog.c_str());
        break;

        case MsgType::FuncEntry:
            ALOGD("[--->] %s", textToLog.c_str());
        break;

        case MsgType::FuncExit:
            ALOGD("[<---] %s", textToLog.c_str());
        break;

        case MsgType::Error:
            ALOGE("%s", textToLog.c_str());
        break;

        case MsgType::Info:
            ALOGI("%s", textToLog.c_str());
        break;

        case MsgType::Warning:
            ALOGW("%s", textToLog.c_str());
        break;

        default:
        break;
    }
}

} // namespace logger
} // namespace nlab

#endif // NLAB_LOGGER_LOGCAT_HPP
