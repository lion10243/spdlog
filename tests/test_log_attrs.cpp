#define SPDLOG_ENABLE_LOG_ATTRIBUTES 1

#include "includes.h"
#include <iostream>

class NullSink : public spdlog::sinks::sink
{
public:
    void log(const spdlog::details::log_msg&) override;
    void flush() override;
};

void NullSink::flush() {}

void NullSink::log(const spdlog::details::log_msg& msg)
{
    // fake sink output
    //std::cout << msg.formatted.str() << std::endl;
    {
        auto iter = msg.attrs.find("param_bool");
        if (iter == msg.attrs.end())
            FAIL("log message missing 'param_bool' attribute");
        REQUIRE( spdlog::attrval::get<bool>( iter->second ) );
    }
    {
        auto iter = msg.attrs.find("param_int");
        if (iter == msg.attrs.end())
            FAIL("log message missing 'param_int' attribute");
        REQUIRE( 42 == spdlog::attrval::get<int>( iter->second ) );
    }
    {
        auto iter = msg.attrs.find("param_long");
        if (iter == msg.attrs.end())
            FAIL("log message missing 'param_long' attribute");
        REQUIRE( 42 == spdlog::attrval::get<long>( iter->second ) );
    }
    {
        auto iter = msg.attrs.find("param_float");
        if (iter == msg.attrs.end())
            FAIL("log message missing 'param_float' attribute");
        REQUIRE( 42.5 == spdlog::attrval::get<float>( iter->second ) );
    }
    {
        auto iter = msg.attrs.find("param_double");
        if (iter == msg.attrs.end())
            FAIL("log message missing 'param_double' attribute");
        REQUIRE( 42.5 == spdlog::attrval::get<double>( iter->second ) );
    }
    {
        auto iter = msg.attrs.find("param_string");
        if (iter == msg.attrs.end())
            FAIL("log message missing 'param_string' attribute");
        REQUIRE("spdlog feature test" == spdlog::attrval::get<std::string>( iter->second ) );
    }
}

TEST_CASE("log attr tests", "[log_attrs]")
{
    // see "common.h" for supported types
    spdlog::attributes_type attrs {
        {"param_bool", true},
        {"param_int", 42},
        {"param_long", 42l},
        {"param_float", 42.5f},
        {"param_double", 42.5},
        {"param_string", std::string("spdlog feature test")},
    };

    auto oss_sink = std::make_shared<NullSink>();
    spdlog::logger oss_logger("attr_tester", oss_sink);
    oss_logger.set_level(spdlog::level::info);

    // see NullSink::log above for attribute value checks
    oss_logger.info(attrs, "Log attributes with log message");

    REQUIRE(true);
}

