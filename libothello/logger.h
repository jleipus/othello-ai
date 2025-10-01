//
// Created by joris on 9/29/25.
//

#ifndef OTHELLO_AI_LOGGER_H
#define OTHELLO_AI_LOGGER_H

#include <spdlog/spdlog.h>

class Logger {
public:
    static void set_level(const int level) {
        const auto spd_level = static_cast<spdlog::level::level_enum>(level);
        spdlog::set_level(spd_level);
    }

    template<typename... Args>
    static void debug(fmt::format_string<Args...> fmt, Args &&... args) {
        spdlog::debug(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void info(fmt::format_string<Args...> fmt, Args &&... args) {
        spdlog::info(fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void error(fmt::format_string<Args...> fmt, Args &&... args) {
        spdlog::error(fmt, std::forward<Args>(args)...);
    }
};

#endif  // OTHELLO_AI_LOGGER_H
