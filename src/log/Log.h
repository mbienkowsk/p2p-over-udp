#ifndef LOG_H
#define LOG_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>


inline void setup_logger(bool log_to_file) {
    if (log_to_file) {
        // Create a file sink
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logfile.log", true);
        spdlog::set_default_logger(std::make_shared<spdlog::logger>("file_logger", file_sink));
    } else {
        // Create a console sink
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        spdlog::set_default_logger(std::make_shared<spdlog::logger>("console_logger", console_sink));
    }

    spdlog::set_level(spdlog::level::info);
    spdlog::info("Logger initialized");
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");

    spdlog::flush_on(spdlog::level::info);
}

#endif // LOG_H