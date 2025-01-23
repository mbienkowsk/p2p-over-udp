#ifndef LOG_H
#define LOG_H

#include <memory>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <string>

inline void setup_logger(bool log_to_file, std::string filename) {
    if (log_to_file) {
        // Create a file sink
        auto file_sink =
            std::make_shared<spdlog::sinks::basic_file_sink_mt>(filename, true);
        spdlog::set_default_logger(
            std::make_shared<spdlog::logger>(filename, file_sink));
    } else {
        // Create a console sink
        auto console_sink =
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        spdlog::set_default_logger(
            std::make_shared<spdlog::logger>("console_logger", console_sink));
    }

    spdlog::set_level(spdlog::level::trace);
    spdlog::info("Logger initialized");
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");

    spdlog::flush_on(spdlog::level::trace);
}

#endif // LOG_H