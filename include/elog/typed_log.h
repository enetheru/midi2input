// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_TYPED_LOG_H_
#define ELOG_TYPED_LOG_H_

#include <cstddef>
#include <sstream>
#include <string>
#include "logger.h"
#include "logger_factory.h"
#include "put_as_string.h"
#include "type_info.h"

namespace LOG {

class TypedLog {
 public:
  TypedLog(TypeInfo type_info,
           int verbosity,
           const char* source_file_name,
           int line_number,
           Logger* logger = NULL)
      : logger_(logger ? *logger : GetLogger()),
        type_info_(type_info),
        verbosity_(verbosity),
        source_file_name_(source_file_name),
        line_number_(line_number) {
  }

  TypedLog(const TypedLog& typed_log)
      : logger_(typed_log.logger_),
        type_info_(typed_log.type_info_),
        verbosity_(typed_log.verbosity_),
        source_file_name_(typed_log.source_file_name_),
        line_number_(typed_log.line_number_) {
  }

  template <typename T>
  TypedLog& operator<<(const T& t) {
    PutAsString(t, stream_);
    return *this;
  }

  TypedLog& GetReference() {
    return *this;
  }

  void PushMessage() const {
    logger_.PushTypedMessage(type_info_, verbosity_,
                             source_file_name_, line_number_, stream_.str());
  }

 private:
  Logger& logger_;
  std::ostringstream stream_;
  TypeInfo type_info_;
  int verbosity_;
  const char* source_file_name_;
  int line_number_;
};

}  // namespace LOG

#endif  // ELOG_TYPED_LOG_H_
