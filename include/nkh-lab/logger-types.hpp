#ifndef NLAB_LOGGER_TYPES_HPP
#define NLAB_LOGGER_TYPES_HPP

namespace nlab {
namespace logger {

enum class MsgType
{
    Info,
    Warning,
    Error,
    Debug,
    FuncEntry,
    FuncExit
};

template <MsgType type>
inline const char* getMsgTypeName()
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

} // namespace logger
} // namespace nlab

#endif // NLAB_LOGGER_TYPES_HPP
