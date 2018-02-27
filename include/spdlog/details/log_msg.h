//
// Copyright(c) 2015 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#pragma once

#include "../common.h"
#include "../details/os.h"


#include <map>
#include <string>
#include <utility>

namespace spdlog
{
namespace details
{
struct log_msg
{
    log_msg() = default;
    log_msg(const std::string *loggers_name, level::level_enum lvl) :
        logger_name(loggers_name),
        level(lvl),
        msg_id(0)
    {
#ifndef SPDLOG_NO_DATETIME
        time = os::now();
#endif

#ifndef SPDLOG_NO_THREAD_ID
        thread_id = os::thread_id();
#endif

#ifdef SPDLOG_ENABLE_ATTRIBUTE_LOGGER
        attrs = attributes_type();
#endif
    }

#ifdef SPDLOG_ENABLE_ATTRIBUTE_LOGGER
    log_msg(const std::string *loggers_name, level::level_enum lvl, attributes_type&& data) :
        logger_name(loggers_name),
        level(lvl),
        msg_id(0),
        attrs(data)
    {
#ifndef SPDLOG_NO_DATETIME
        time = os::now();
#endif

#ifndef SPDLOG_NO_THREAD_ID
        thread_id = os::thread_id();
#endif
    }
#endif

    log_msg(const log_msg& other)  = delete;
    log_msg& operator=(log_msg&& other) = delete;
    log_msg(log_msg&& other) = delete;


    const std::string *logger_name;
    level::level_enum level;
    log_clock::time_point time;
    size_t thread_id;
    fmt::MemoryWriter raw;
    fmt::MemoryWriter formatted;
    size_t msg_id;
#ifdef SPDLOG_ENABLE_ATTRIBUTE_LOGGER
    attributes_type attrs;
#endif
};
}
}