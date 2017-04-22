#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <sstream>
#include <string.h>

/* Logger Settings */
#define LOGGER_ENNABLED // comment out if logging unnneded

//Output
#define LOGGER_OUTPUT COUT // COUT, FILE, CUSTOM

namespace logger {

enum class MsgType
{
    Info,
    Warning,
    Error,
    Debug
};

class Msg
{
    std::stringstream streamTolog;

public:
    Msg(const MsgType msgType, const char* function, const char* file, const int line);

    ~Msg();

    template<typename T>
    Msg& operator << (const T& p)
    {
       streamTolog << p;

       return *this;
    }
private:
    const char* getMsgTypeName(const MsgType msgType);

};

class Func
{
public:
    Func(){};
};

}

// Macros to log messages
#ifdef LOGGER_ENNABLED

#define LOG_INFO   logger::Msg(logger::MsgType::Info,    __FUNCTION__, __FILE__, __LINE__)
#define LOG_WANING logger::Msg(logger::MsgType::Warning, __FUNCTION__, __FILE__, __LINE__)
#define LOG_ERROR  logger::Msg(logger::MsgType::Error,   __FUNCTION__, __FILE__, __LINE__)
#define LOG_DEBUG  logger::Msg(logger::MsgType::Debug,   __FUNCTION__, __FILE__, __LINE__)

#else

// TODO: Needed solution to comment out given line for messages: LOG_INFO, LOG_WARNING ...


#endif


#endif
