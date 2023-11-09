#include "core/log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/cfg/env.h>

namespace counterstrikesharp {
std::shared_ptr<spdlog::logger> Log::m_core_logger;

void Log::Init() {
    std::vector<spdlog::sink_ptr> logSinks;
#ifdef _WIN32
    auto colorSink = std::make_shared<spdlog::sinks::wincolor_stderr_sink_mt>();
    colorSink->set_color(spdlog::level::trace, 6);
#else
    auto colorSink = std::make_shared<spdlog::sinks::ansicolor_stderr_sink_mt>();
    colorSink->set_color(spdlog::level::trace, colorSink->yellow);
#endif

    logSinks.emplace_back(colorSink);
    logSinks.emplace_back(
        std::make_shared<spdlog::sinks::basic_file_sink_mt>("counterstrikesharp.log", true));

    logSinks[0]->set_pattern("%^[%T.%e] %n: %v%$");
    logSinks[1]->set_pattern("[%T.%e] [%l] %n: %v");

    m_core_logger = std::make_shared<spdlog::logger>("CSSharp", begin(logSinks), end(logSinks));
    spdlog::register_logger(m_core_logger);
    m_core_logger->set_level(spdlog::level::info);
    m_core_logger->flush_on(spdlog::level::info);

    spdlog::cfg::load_env_levels();
}

void Log::Close() {
    spdlog::drop("CSSharp");
    m_core_logger = nullptr;
}
}  // namespace counterstrikesharp
