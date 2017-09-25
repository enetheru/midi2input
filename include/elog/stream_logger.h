// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_STREAM_LOGGER_H_
#define ELOG_STREAM_LOGGER_H_

#include "config.h"

#include <iostream>
#ifdef ELOG_I_USE_TR1_HEADER
# include <tr1/unordered_map>
#else
# include <unordered_map>
#endif
#include "atomic.h"
#include "logger.h"
#include "mutex.h"

namespace LOG {

class StreamLogger : public Logger {
 public:
  explicit StreamLogger(std::ostream& stream = std::clog)
      : stream_(stream),
        level_(INFO) {
  }

  void set_level(LogLevel level) {
    level_ = level;
  }

  template <typename T>
  void SetTypeVerbosity(int verbosity) {
    SetTypeVerbosity(TypeInfo(Type<T>()), verbosity);
  }

  void SetTypeVerbosity(TypeInfo type_info, int verbosity) {
    MutexLock lock(verbosity_mutex_);
    verbosities_[type_info] = verbosity;
  }

  void ResetVerbosities() {
    verbosities_.clear();
  }

  virtual void PushRawMessage(LogLevel level, const std::string& message) {
    if (!IsLogLevelSevereEnough(level, level_)) return;
    stream_ << message << std::endl;
  }

  virtual void PushMessage(LogLevel level,
                           const char* source_file_name,
                           int line_number,
                           const std::string& message) {
    if (!IsLogLevelSevereEnough(level, level_)) return;
    PushMessageWithoutCheck(level, source_file_name, line_number, message);
  }

  virtual void PushFatalMessageAndThrow(const char* source_file_name,
                                        int line_number,
                                        const std::string& message) {
    PushMessageWithoutCheck(FATAL, source_file_name, line_number, message);
    throw FatalLogError();
  }

  virtual void PushCheckMessageAndThrow(const char* source_file_name,
                                        int line_number,
                                        const std::string& message) {
    PushMessageWithoutCheck(CHECK, source_file_name, line_number, message);
    throw CheckError();
  }

  virtual void PushTypedMessage(TypeInfo type_info,
                                int verbosity,
                                const char* source_file_name,
                                int line_number,
                                const std::string& message) {
    {
      MutexLock verbosity_lock(verbosity_mutex_);
      const int type_verbosity = verbosities_[type_info];
      if (IsVerboseEnough(verbosity, type_verbosity)) return;
    }
    MutexLock lock(push_message_mutex_);
    OutputTypedMessageHeader(type_info, verbosity, stream_);
    OutputFileLine(source_file_name, line_number, stream_);
    stream_ << message << std::endl;
  }

 private:
  void PushMessageWithoutCheck(LogLevel level,
                               const char* source_file_name,
                               int line_number,
                               const std::string& message) {
    MutexLock lock(push_message_mutex_);
    OutputLogLevelName(level, stream_);
    OutputFileLine(source_file_name, line_number, stream_);
    stream_ << message << std::endl;
  }

  std::ostream& stream_;
  Mutex push_message_mutex_;
  Mutex verbosity_mutex_;
  LogLevel level_;
  std::tr1::unordered_map<TypeInfo, int, TypeInfo::Hash> verbosities_;
};

}  // namespace LOG

#endif  // ELOG_STREAM_LOGGER_H_
