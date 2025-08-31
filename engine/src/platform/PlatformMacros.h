#pragma once
#include "ocf/core/logger.h"
#include <stdint.h>
#include <string.h>

#define OCF_SAFE_DELETE(p) \
    do                     \
    {                      \
        delete (p);        \
        (p) = nullptr;     \
    } while (0)

#define OCF_SAFE_DELETE_ARRAY(p) \
    do                           \
    {                            \
        if (p)                   \
        {                        \
            delete[](p);         \
            (p) = nullptr;       \
        }                        \
    } while (0)

#define OCF_SAFE_RELEASE(p) \
    do                      \
    {                       \
        if(p)               \
        {                   \
            (p)->release(); \
        }                   \
    } while (0);            \

#define OCF_SAFE_RETAIN(p)  \
    do                      \
    {                       \
        if(p)               \
        {                   \
            (p)->retain();  \
        }                   \
    } while (0);            \

#define OCF_SAFE_FREE(p)  \
    do                    \
    {                     \
        free(p);          \
        (p) = nullptr;    \
    } while (0)

#define OCF_BREAK_IF(cond) \
    if (cond)              \
    break

#define OCF_LOG_TRACE(...)                                                                         \
    ocf::logger::Logger::getInstance().log(ocf::logger::LogLevel::Trace, __VA_ARGS__)
#define OCF_LOG_DEBUG(...)                                                                         \
    ocf::logger::Logger::getInstance().log(ocf::logger::LogLevel::Debug, __VA_ARGS__)
#define OCF_LOG_INFO(...)                                                                          \
    ocf::logger::Logger::getInstance().log(ocf::logger::LogLevel::Info, __VA_ARGS__)
#define OCF_LOG_WARN(...)                                                                          \
    ocf::logger::Logger::getInstance().log(ocf::logger::LogLevel::Warn, __VA_ARGS__)
#define OCF_LOG_ERROR(...)                                                                         \
    ocf::logger::Logger::getInstance().log(ocf::logger::LogLevel::Error, __VA_ARGS__)
#define OCF_LOG_FATAL(...)                                                                         \
    ocf::logger::Logger::getInstance().log(ocf::logger::LogLevel::Fatal, __VA_ARGS__)
