#ifndef NLAB_LOGGER_MSGTYPE_HPP
#define NLAB_LOGGER_MSGTYPE_HPP

#include <sstream>
#include <string.h>

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

class Msg
{
public:
    Msg(MsgType type, const char* fileFullPath, const char* functionName, size_t line);

    // disallow copying
    Msg(const Msg&) = delete;
    Msg& operator=(const Msg&) = delete;


    // disallow moving
    Msg(Msg&&) = delete;
    Msg& operator=(Msg&&) = delete;

    ~Msg();

    template<typename T>
    Msg& operator << (const T& p)
    {
        if (isAllowedToLog()) mStreamTolog << p;

        return *this;
    }

private:
    bool isAllowedToLog();

private:
    MsgType mType;
    std::stringstream mStreamTolog;
};

class FunctionEnterExit
{
public:
    FunctionEnterExit(const char* fileFullPath, const char* functionName, size_t line);

    // disallow copying
    FunctionEnterExit(const FunctionEnterExit&) = delete;
    FunctionEnterExit& operator=(const FunctionEnterExit&) = delete;

    // disallow moving
    FunctionEnterExit(FunctionEnterExit&&) = delete;
    FunctionEnterExit& operator=(FunctionEnterExit&&) = delete;

    ~FunctionEnterExit();

private:
    std::stringstream mStreamTolog;
};

/* To log format string messages
 *
 * Example:
 * logger::logMsg(logger::msgType::Info, __FUNCTION__, __FILE__, __LINE__, "Test %d %s %c", 888, "str", 'c' );
 */
int logMsg(logger::MsgType type, const char* fileFullPath, const char* functionName, size_t line, const char* fmt, ...);

} // namespace logger
} // namespace nlab

#endif // NLAB_LOGGER_MSGTYPE_HPP
