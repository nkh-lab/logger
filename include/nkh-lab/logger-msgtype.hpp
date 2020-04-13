#ifndef NLAB_LOGGER_MSGTYPE_HPP
#define NLAB_LOGGER_MSGTYPE_HPP

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

} // namespace logger
} // namespace nlab

#endif // NLAB_LOGGER_MSGTYPE_HPP
