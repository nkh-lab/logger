#ifndef NLAB_LOGGER_IVI_HPP
#define NLAB_LOGGER_IVI_HPP

#include "logger-msgtype.hpp"
#include "ivi-logging-config.hpp"

#include <string>

namespace nlab {
namespace logger {

inline void logToIvi(MsgType logType, std::string textToLog)
{
    try
    {
        switch (logType)
        {
            case MsgType::Debug:
            case MsgType::FuncEntry:
            case MsgType::FuncExit:
                log_debug() << textToLog;
                break;

            case MsgType::Error:
                log_error() << textToLog;
                break;

            case MsgType::Info:
                log_info() << textToLog;
                break;

            case MsgType::Warning:
                log_warning() << textToLog;
                break;

            default:
                break;
        }
    }
    catch(...){}
}

} // namespace logger
} // namespace nlab

#endif // NLAB_LOGGER_IVI_HPP
