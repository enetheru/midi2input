// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_LOGGER_FACTORY_H_
#define ELOG_LOGGER_FACTORY_H_

#include "atomic.h"
#include "singleton.h"
#include "stream_logger.h"
#include "util.h"

namespace LOG {

class LoggerFactory : Noncopyable {
 public:
  LoggerFactory()
      : logger_(&GetDefaultLogger()) {
  }

  Logger& logger() const {
    return *logger_;
  }

  void set_logger(Logger& logger) {
    AtomicSet(logger_, &logger);
  }

  void Reset() {
    set_logger(GetDefaultLogger());
  }

 private:
  static Logger& GetDefaultLogger() {
    return Singleton<StreamLogger>::Get();
  }

  Logger* volatile logger_;
};

inline Logger& GetLogger() {
  return Singleton<LoggerFactory>::Get().logger();
}

inline void SetLogger(Logger& logger) {
  Singleton<LoggerFactory>::Get().set_logger(logger);
}

inline void UseDefaultLogger() {
  Singleton<LoggerFactory>::Get().Reset();
}


inline void SetDefaultLoggerLevel(LogLevel level) {
  Singleton<StreamLogger>::Get().set_level(level);
}

template <typename T>
inline void SetDefaultLoggerVerbosity(int verbosity) {
  Singleton<StreamLogger>::Get().SetTypeVerbosity(
      TypeInfo(Type<T>()), verbosity);
}

}  // namespace LOG

#endif  // ELOG_LOGGER_FACTORY_H_
