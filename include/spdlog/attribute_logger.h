//
// Copyright(c) 2015 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#pragma once

// Specialisation of logger to allow log attributes (extra data, metadata etc) with log message.
// Upon each log write the logger:
// 1. Checks if its log level is enough to log the message
// 2. Format the message using the formatter function
// 3. Sets the log attributes on the log message
// 4. Pass the formatted message to its sinks, along with the extra log attributes, to perform the actual logging

#include "common.h"
#include "logger.h"

#include <functional>
#include <map>
#include <memory>
#include <string>

namespace spdlog
{

using attributes_type = std::map<std::string, std::string>;

class attribute_logger SPDLOG_FINAL : public logger
{
public:
    // inherit ctors from parent
    using logger::logger;

    template <typename... Args> void log(level::level_enum lvl, attributes_type& attrs, const char* fmt, const Args&... args);
    template <typename... Args> void log(level::level_enum lvl, attributes_type& attrs, const char* msg);
    template <typename... Args> void trace(attributes_type& attrs, const char* fmt, const Args&... args);
    template <typename... Args> void debug(attributes_type& attrs, const char* fmt, const Args&... args);
    template <typename... Args> void info(attributes_type& attrs, const char* fmt, const Args&... args);
    template <typename... Args> void warn(attributes_type& attrs, const char* fmt, const Args&... args);
    template <typename... Args> void error(attributes_type& attrs, const char* fmt, const Args&... args);
    template <typename... Args> void critical(attributes_type& attrs, const char* fmt, const Args&... args);

    template <typename T> void log(level::level_enum lvl, attributes_type& attrs, const T&);
    template <typename T> void trace(attributes_type& attrs, const T&);
    template <typename T> void debug(attributes_type& attrs, const T&);
    template <typename T> void info(attributes_type& attrs, const T&);
    template <typename T> void warn(attributes_type& attrs, const T&);
    template <typename T> void error(attributes_type& attrs, const T&);
    template <typename T> void critical(attributes_type& attrs, const T&);
};

}

#include "details/attribute_logger_impl.h"
