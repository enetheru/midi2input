// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#include <iostream>
#include <sstream>
#include <gtest/gtest.h>
#include "stream_logger.h"
using namespace std;

namespace LOG {

namespace {

static const LogLevel kLevels[] = { INFO, WARN, ERROR, FATAL };
static const size_t kNumLevels = sizeof(kLevels) / sizeof(kLevels[0]);
static const char* kMessage = "message";

void VerifyPushMessage(LogLevel level) {
  static const char* kSourceFileName = "source file name";
  static const int kLineNumber = 10;
  const char* log_level_name = LogLevelNames::names[level];

  std::ostringstream stream;
  StreamLogger logger(stream);

  if (level == FATAL) {
    EXPECT_THROW(
        logger.PushFatalMessageAndThrow(kSourceFileName, kLineNumber, kMessage),
        FatalLogError);
  } else if (level == CHECK) {
    EXPECT_THROW(
        logger.PushCheckMessageAndThrow(kSourceFileName, kLineNumber, kMessage),
        CheckError);
  } else {
    logger.PushMessage(level, kSourceFileName, kLineNumber, kMessage);
  }

  std::string full_message = stream.str();
  EXPECT_NE(std::string::npos, full_message.find(log_level_name));
  EXPECT_NE(std::string::npos, full_message.find(kMessage));
}

}  // anonymous namespace

TEST(StreamLoggerTest, PushMessageINFO) {
  VerifyPushMessage(INFO);
}

TEST(StreamLoggerTest, PushMessageWARN) {
  VerifyPushMessage(WARN);
}

TEST(StreamLoggerTest, PushMessageERROR) {
  VerifyPushMessage(ERROR);
}

TEST(StreamLoggerTest, PushMessageFATAL) {
  VerifyPushMessage(FATAL);
}

TEST(StreamLoggerTest, PushMessageCHECK) {
  VerifyPushMessage(CHECK);
}

// TODO(S.Tokui): Write multi-thread test.

}  // namespace LOG
