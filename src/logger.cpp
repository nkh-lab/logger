#include "logger.hpp"

namespace logger {

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

Msg::Msg(const MsgType msgType, const char* function, const char* file, const int line)
{
#if LOGGER_OUTPUT == COUT

    streamTolog << getMsgTypeName(msgType) << " ";

#endif

    streamTolog << " " << getFileNameFromPath(file) << " " << function << "() line " << line << ": ";
}

Msg::~Msg()
{
    streamTolog << "\n";

#if LOGGER_OUTPUT == COUT

    std::cout << streamTolog.str();

#endif
}

const char* Msg::getMsgTypeName(const MsgType msgType)
{
    const char* ret;

    switch (msgType)
    {
        case MsgType::Debug:
            ret = "LOG_DEBUG";
            break;

        case MsgType::Error:
            ret = "LOG_ERROR";
            break;

        case MsgType::Info:
            ret = "LOG_INFO";
            break;

        case MsgType::Warning:
            ret = "LOG_WARNING";
            break;

        default:
            break;
    }

    return ret;
}

}
