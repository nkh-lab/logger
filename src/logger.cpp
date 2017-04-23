#include "logger.hpp"

namespace logger {

const char* getMsgTypeName(const LogType msgType);
const char* getFileNameFromPath(const char* path);

Msg::Msg(const LogType logType, const char* function, const char* file, const int line)
{
#if LOGGER_OUTPUT == COUT

    StreamTolog << getMsgTypeName(logType) << " ";

#endif

    StreamTolog << " " << getFileNameFromPath(file) << " " << function << "() line " << line << ": ";
}

Msg::~Msg()
{
    StreamTolog << "\n";

#if LOGGER_OUTPUT == COUT

    std::cout << StreamTolog.str();

#endif
}

Func::Func(const LogType logType, const char* function, const char* file, const int line)
{
    std::stringstream streamTolog;

#if LOGGER_OUTPUT == COUT

    streamTolog << getMsgTypeName(logType) << " ";

#endif

    streamTolog << " " << getFileNameFromPath(file) << " " << function << "() line " << line << ":\n";

#if LOGGER_OUTPUT == COUT

    std::cout << streamTolog.str();

#endif
}

const char* getMsgTypeName(const LogType logType)
{
    const char* ret;

    switch (logType)
    {
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

        default:
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

}
