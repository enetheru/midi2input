// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_LOGGER_H_
#define ELOG_LOGGER_H_

#include <exception>
#include <string>

// Identifier ERROR conflicts macro
#ifdef _WIN32
# include <windows.h>
# undef ERROR
#endif

#include "type_info.h"
#include "util.h"

namespace LOG {

enum LogLevel {
  INFO,
  WARN,
  ERROR,
  FATAL,
  CHECK
};

inline bool IsLogLevelSevereEnough(LogLevel log_level, LogLevel logger_level) {
  return log_level >= logger_level;
}

inline bool IsVerboseEnough(int log_verbosity, int logger_verbosity) {
  return log_verbosity > logger_verbosity;
}

template <AvoidODR>
struct LogLevelNamesTemplate {
  static const char* names[5];
};

template <AvoidODR N>
const char* LogLevelNamesTemplate<N>::names[5] = {
  "INFO",
  "WARN",
  "ERROR",
  "FATAL",
  "CHECK"
};

typedef LogLevelNamesTemplate<AVOID_ODR> LogLevelNames;

struct FatalLogError : virtual std::exception {};
struct CheckError : virtual std::exception {};

class Logger {
 public:
  template <typename OutputStream>
  static void OutputLogLevelName(LogLevel level, OutputStream& stream) {
    const char* log_level_name = LogLevelNames::names[level];
    stream << "[" << log_level_name << "] ";
  }

  template <typename OutputStream>
  static void OutputTypedMessageHeader(TypeInfo type_info,
                                       int verbosity,
                                       OutputStream& stream) {
    const Demangle demangle = type_info.GetTypeName();
    stream << "[" << demangle.GetName() << "(" << verbosity << ")] ";
  }

  template <typename OutputStream>
  static void OutputFileLine(const char* source_file_name,
                             int line_number,
                             OutputStream& stream) {
    //stream << source_file_name << "(" << line_number << "): ";
  }

  virtual ~Logger() {}

  virtual void PushRawMessage(LogLevel level, const std::string& message) = 0;

  virtual void PushMessage(LogLevel level,
                           const char* source_file_name,
                           int line_number,
                           const std::string& message) = 0;

  virtual void PushFatalMessageAndThrow(const char* source_file_name,
                                        int line_number,
                                        const std::string& message) = 0;

  virtual void PushCheckMessageAndThrow(const char* source_file_name,
                                        int line_number,
                                        const std::string& message) = 0;

  virtual void PushTypedMessage(TypeInfo type_info,
                                int verbosity,
                                const char* source_file_name,
                                int line_number,
                                const std::string& message) = 0;
};

}  // namespace LOG

#endif  // ELOG_LOGGER_H_
