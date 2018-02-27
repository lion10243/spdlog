//
// Copyright(c) 2015 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

#pragma once

#include "../attribute_logger.h"
#include "../sinks/stdout_sinks.h"

#include <memory>
#include <string>


template <typename... Args>
inline void spdlog::attribute_logger::log(level::level_enum lvl, attributes_type& attrs, const char* fmt, const Args&... args)
{
    if (!should_log(lvl)) return;

    try
    {
        details::attr_log_msg log_msg(&_name, lvl, std::move(attrs));

#if defined(SPDLOG_FMT_PRINTF)
        fmt::printf(log_msg.raw, fmt, args...);
#else
        log_msg.raw.write(fmt, args...);
#endif
        _sink_it(log_msg);
    }
    catch (const std::exception &ex)
    {
        _err_handler(ex.what());
    }
    catch(...)
    {
        _err_handler("Unknown exception in attribute_logger " + _name);
        throw;
    }
}

template <typename... Args>
inline void spdlog::attribute_logger::log(level::level_enum lvl, attributes_type& attrs, const char* msg)
{
    if (!should_log(lvl)) return;
    try
    {
        details::attr_log_msg log_msg(&_name, lvl, std::move(attrs));
        log_msg.raw << msg;
        _sink_it(log_msg);
    }
    catch (const std::exception &ex)
    {
        _err_handler(ex.what());
    }
    catch (...)
    {
        _err_handler("Unknown exception in attribute_logger " + _name);
        throw;
    }
}

template<typename T>
inline void spdlog::attribute_logger::log(level::level_enum lvl, attributes_type& attrs, const T& msg)
{
    if (!should_log(lvl)) return;
    try
    {
        details::attr_log_msg log_msg(&_name, lvl, std::move(attrs));
        log_msg.raw << msg;
        _sink_it(log_msg);
    }
    catch (const std::exception &ex)
    {
        _err_handler(ex.what());
    }
    catch (...)
    {
        _err_handler("Unknown exception in attribute_logger " + _name);
        throw;
    }
}

template <typename... Args>
inline void spdlog::attribute_logger::trace(attributes_type& attrs, const char* fmt, const Args&... args)
{
    log(level::trace, attrs, fmt, args...);
}

template <typename... Args>
inline void spdlog::attribute_logger::debug(attributes_type& attrs, const char* fmt, const Args&... args)
{
    log(level::debug, attrs, fmt, args...);
}

template <typename... Args>
inline void spdlog::attribute_logger::info(attributes_type& attrs, const char* fmt, const Args&... args)
{
    log(level::info, attrs, fmt, args...);
}

template <typename... Args>
inline void spdlog::attribute_logger::warn(attributes_type& attrs, const char* fmt, const Args&... args)
{
    log(level::warn, attrs, fmt, args...);
}

template <typename... Args>
inline void spdlog::attribute_logger::error(attributes_type& attrs, const char* fmt, const Args&... args)
{
    log(level::err, attrs, fmt, args...);
}

template <typename... Args>
inline void spdlog::attribute_logger::critical(attributes_type& attrs, const char* fmt, const Args&... args)
{
    log(level::critical, attrs, fmt, args...);
}

template<typename T>
inline void spdlog::attribute_logger::trace(attributes_type& attrs, const T& msg)
{
    log(level::trace, attrs, msg);
}

template<typename T>
inline void spdlog::attribute_logger::debug(attributes_type& attrs, const T& msg)
{
    log(level::debug, attrs, msg);
}

template<typename T>
inline void spdlog::attribute_logger::info(attributes_type& attrs, const T& msg)
{
    log(level::info, attrs, msg);
}

template<typename T>
inline void spdlog::attribute_logger::warn(attributes_type& attrs, const T& msg)
{
    log(level::warn, attrs, msg);
}

template<typename T>
inline void spdlog::attribute_logger::error(attributes_type& attrs, const T& msg)
{
    log(level::err, attrs, msg);
}

template<typename T>
inline void spdlog::attribute_logger::critical(attributes_type& attrs, const T& msg)
{
    log(level::critical, attrs, msg);
}
