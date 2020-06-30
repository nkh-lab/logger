#ifndef NLAB_LOGGER_HPP
#define NLAB_LOGGER_HPP

#include "logger-impl.hpp"

// clang-format off

/*
 * Main macros to log
 */
#define LOG_INF nlab::logger::Msg<nlab::logger::MsgType::Info>   (__FILE__, __FUNCTION__, __LINE__)
#define LOG_WRN nlab::logger::Msg<nlab::logger::MsgType::Warning>(__FILE__, __FUNCTION__, __LINE__)
#define LOG_ERR nlab::logger::Msg<nlab::logger::MsgType::Error>  (__FILE__, __FUNCTION__, __LINE__)
#define LOG_DBG nlab::logger::Msg<nlab::logger::MsgType::Debug>  (__FILE__, __FUNCTION__, __LINE__)

#ifndef LOG_FUNCTION_ENTER_EXIT_DISABLED
#define LOG_FNC nlab::logger::FunctionEnterExit lf(__FILE__, __FUNCTION__, __LINE__)
#else
#define LOG_FNC
#endif

#define CHECK(value, ...) (value ? true : (nlab::logger::logMsg<nlab::logger::MsgType::Error>(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__) && false))

// clang-format on

#endif // NLAB_LOGGER_HPP
