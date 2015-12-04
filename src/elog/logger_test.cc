// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#include <sstream>
#include <string>
#include <gtest/gtest.h>
#include "logger.h"

namespace LOG {

namespace {

class PseudoOutputStream {
 public:
  PseudoOutputStream& operator<<(const char* str) {
    buffer_ += str;
    return *this;
  }

  const std::string& GetBuffer() const {
    return buffer_;
  }

 private:
  std::string buffer_;
};

}  // anonymous namespace

TEST(LoggerTest, OutputLogLevelName) {
  PseudoOutputStream stream;
  Logger::OutputLogLevelName(INFO, stream);
  EXPECT_EQ("[INFO] ", stream.GetBuffer());
}

}  // namespace LOG
