#define SPDLOG_ENABLE_ATTRIBUTE_LOGGER 1

#include "includes.h"

class FakeLogDaemon {
public:
    using metadata_type = std::map<std::string, std::string>;
    void log(std::string const& msg, metadata_type const& map);
};

void FakeLogDaemon::log(std::string const& msg, metadata_type const& extra) {
    // in real app, daemon would receive the log message string, plus a map of key-val extra values
    // an example:
    /*
    auto json = {
        {"src-log", extra["logger"]},
        {"time", extra["time"]},
        {"message", msg},
        {"context_param_one", extra["ctx_param_one"]},
        {"context_param_two", extra["ctx_param_two"]},
        {"severity", msg.level},
        {"msgid", msg.msg_id}
        ...
    }_json;

    daemon->write(json);
    */
}

class FakeLogDaemonSink : public spdlog::sinks::sink
{
public:
    void log(const spdlog::details::log_msg&) override;
    void flush() override;
private:
    FakeLogDaemon fake_daemon_;
};

void FakeLogDaemonSink::flush() {}

void FakeLogDaemonSink::log(const spdlog::details::log_msg& msg)
{
    // first test the message contains the log metadata attributes
    {
        auto iter = msg.attrs.find("ctx_param_one");
        if (iter == msg.attrs.end())
            FAIL("log message missing 'ctx_param_one' attribute");
        REQUIRE( "meta data for log daemon" == iter->second );
    }
    {
        auto iter = msg.attrs.find("ctx_param_two");
        if (iter == msg.attrs.end())
            FAIL("log message missing 'ctx_param_two' attribute");
        REQUIRE( "extra data" == iter->second );
    }
    {
        auto iter = msg.attrs.find("param_string");
        if (iter == msg.attrs.end())
            FAIL("log message missing 'param_string' attribute");
        REQUIRE( "spdlog feature test" == iter->second );
    }

    auto extra = msg.attrs;
    extra["severity"] = spdlog::level::to_str( msg.level );
    extra["msg_id"] = std::to_string( msg.msg_id );
    using namespace std::chrono;
    auto time_units = duration_cast<duration<double, seconds::period>>( msg.time.time_since_epoch() ).count();
    extra["time"] = std::to_string( time_units );

    // pass log message + extra data to fake log daemon.
    fake_daemon_.log(msg.formatted.str(), extra);
}

TEST_CASE("attribute_logger test", "[attribute_logger]")
{
    spdlog::attributes_type metadata {
        {"ctx_param_one", "meta data for log daemon"},
        {"ctx_param_two", "extra data"},
        {"param_string", "spdlog feature test"},
    };

    // construct the logger object
    std::ostringstream oss;
    auto oss_sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(oss);
    spdlog::attribute_logger logger("attr_tester", oss_sink);
    logger.set_level(spdlog::level::info);
    auto formatter = std::make_shared<spdlog::pattern_formatter>("%v");
    logger.set_formatter(formatter);

    // test that attribute_logger can take a key-val map of context data.
    logger.info(metadata, "formatted log message");

    REQUIRE(true);
}

