/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#pragma once

#include <iostream>
#include <string>

namespace {

/**
 *  Enum describing possible levels of logging
 */
enum LogLevel {
  DEBUG,
  INFO,
  WARNING,
  ERROR
};

/**
 *  Corner case logging function when there are no arguments to log.
 *
 *  @param[in] stream Output stream.
 *  @param[in] level Level of logging.
 *  @param[in] fmt Format string.
 */
inline void sprint(std::ostream &stream,
                   LogLevel level,
                   const std::string &fmt) {
  stream << fmt << std::endl;
}

/**
 *  Variadic template type safe logging implementation
 *
 *  @param[in] stream Output stream.
 *  @param[in] level Level of logging.
 *  @param[in] fmt Format string.
 *  @param[in] head Next (head) argument from variadic template argument list.
 *  @param[in] tail Rest of the variadic template arguments.
 */
template <typename T, typename ...Args>
void sprint(std::ostream &stream,
            LogLevel level,
            const std::string &fmt,
            const T &head, Args &&... tail) {
  constexpr char marker[] = "{}";
  std::string::size_type format_marker_pos = fmt.find(marker);
  stream << fmt.substr(0, format_marker_pos);
  if (format_marker_pos != fmt.npos) {
    stream << head;
    sprint(stream, level,
           fmt.substr(format_marker_pos + sizeof(marker) - 1), tail...);
  } else
  { stream << std::endl; }
}

/**
 *  Main logging function. Future compatibility is not guaranteed. DON'T USE IT!
 *
 *  @param[in] level Level of logging.
 *  @param[in] levelStr String with logging level name.
 *  @param[in] fmt Format string.
 *  @param[in] args Variadic template argument list.
 */
template <typename ...Args>
void Log(LogLevel level, const std::string &levelStr,
         const std::string &fmt, Args &&... args) {
  sprint(std::cout, level, levelStr + fmt, args...);
}

/**
 *  Minor macro magic for compile time log string concatenation.
 *  Shouldn't be used in the code directly. Use below macros instead.
 */
#ifndef NO_LOGS
#define LOG(LEVEL, MSG, ...) Log( LEVEL, "[" #LEVEL "] ", MSG, ##__VA_ARGS__ )
#else
#define LOG(LEVEL, MSG, ...)
#endif

/**
 *  Set of defines for easy logging. Only those should be used in engine code.
 *  Future compatibility is guaranteed
 *
 *  Usage:
 *  - Letters D, I, W and E stands for
 *    Debug, Info, Warning and Error respectively.
 *  - First argument contains main message to log.
 *  - Other arguments are optional,
 *    each provided optional argument must override stream << operator.
 *  - For each optional argument {} marker should be placed in main
 *    message string similarly to C-style printf.
 *  - Markers that do not have coresponding arguments will be
 *    treated as normal string.
 *  - Arguments that do not have coresponding markers will be ignored.
 *  - If logs should be disabled compile with NO_LOGS flag enabled
 *
 *  Examples:
 *  - LOGD("debug message");
 *  - LOGI(GetInfoMsg());
 *  - LOGW("Object {} is of size {}", object.getName(), object.getSize());
 *  - LOGE("error {}", error_id);
 */
#define LOGD(MSG, ...) LOG( DEBUG, MSG, ##__VA_ARGS__ )
#define LOGI(MSG, ...) LOG( INFO, MSG, ##__VA_ARGS__ )
#define LOGW(MSG, ...) LOG( WARNING, MSG, ##__VA_ARGS__ )
#define LOGE(MSG, ...) LOG( ERROR, MSG, ##__VA_ARGS__ )

}
