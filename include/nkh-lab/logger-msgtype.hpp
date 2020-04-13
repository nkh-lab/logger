#ifndef NKH_LAB_LOGGER_MSG_TYPE_HPP
#define NKH_LAB_LOGGER_MSG_TYPE_HPP

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

}
#endif // NKH_LAB_LOGGER_MSG_TYPE_HPP
