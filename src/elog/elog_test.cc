// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#include <iostream>
#include <sstream>
#include <string>
#include <gtest/gtest.h>
#include "elog.h"

namespace LOG {

namespace {

const char* kMessage = "message";

class SomeModule {};
class AnotherModule {};

}  // anonymous namespace

class LOGTest : public ::testing::Test {
 public:
  LOGTest()
      : logger_(stream_) {
  }

 protected:
  virtual void SetUp() {
    Reset();
    SetLogger(logger_);
  }

  virtual void TearDown() {
    UseDefaultLogger();
    SetLevel(INFO);
    logger_.ResetVerbosities();
  }

  void Reset() {
    stream_.str("");
  }

  std::string GetMessage() const {
    return stream_.str();
  }

  void SetLevel(LogLevel level) {
    logger_.set_level(level);
  }

  template <typename T>
  void SetVerbosity(int verbosity) {
    const TypeInfo type_info((Type<T>()));
    logger_.SetTypeVerbosity(type_info, verbosity);
  }

  void VerifyLevel(LogLevel level) const {
    const char* log_level_name = LogLevelNames::names[level];
    VerifyMessage(log_level_name);
  }

  void VerifyMessage(const std::string& must_have_me) const {
    const std::string message = GetMessage();
    std::cout << "message: " << message << std::endl;
    EXPECT_NE(std::string::npos, message.find(must_have_me));
  }

  template <typename T>
  void VerifyType() const {
    const TypeInfo type_info((Type<T>()));
    const std::string type_name(type_info.GetTypeName().GetName());
    VerifyMessage(type_name);
  }

  void VerifyEmpty() const {
    const std::string message = GetMessage();
    EXPECT_EQ("", message);
  }

 private:
  std::ostringstream stream_;
  StreamLogger logger_;
};

TEST_F(LOGTest, LevelNULL) {
  LOG();
  VerifyLevel(INFO);
}

TEST_F(LOGTest, LevelINFO) {
  LOG(INFO);
  VerifyLevel(INFO);
}

TEST_F(LOGTest, LevelWARN) {
  LOG(WARN);
  VerifyLevel(WARN);
}

TEST_F(LOGTest, LevelERROR) {
  LOG(ERROR);
  VerifyLevel(ERROR);
}

TEST_F(LOGTest, LevelFATAL) {
  EXPECT_THROW(LOG(FATAL), FatalLogError);
  VerifyLevel(FATAL);
}

TEST_F(LOGTest, LevelCHECK) {
  EXPECT_THROW(CHECK(false), CheckError);
  VerifyLevel(CHECK);
}

TEST_F(LOGTest, MessageNULL) {
  LOG() << kMessage;
  VerifyMessage(kMessage);
}

TEST_F(LOGTest, MessageINFO) {
  LOG(INFO) << kMessage;
  VerifyMessage(kMessage);
}

TEST_F(LOGTest, MessageWARN) {
  LOG(WARN) << kMessage;
  VerifyMessage(kMessage);
}

TEST_F(LOGTest, MessageERROR) {
  LOG(ERROR) << kMessage;
  VerifyMessage(kMessage);
}

TEST_F(LOGTest, MessageFATAL) {
  EXPECT_THROW(LOG(FATAL) << kMessage, FatalLogError);
  VerifyMessage(kMessage);
}

TEST_F(LOGTest, MessageCHECK) {
  EXPECT_THROW(CHECK(false) << kMessage, CheckError);
  VerifyMessage(kMessage);
}

TEST_F(LOGTest, LevelNotHighEnough) {
  SetLevel(WARN);
  LOG(INFO) << kMessage;
  VerifyEmpty();
}

TEST_F(LOGTest, LevelHighEnough) {
  SetLevel(ERROR);
  LOG(ERROR);
  VerifyLevel(ERROR);
}


TEST_F(LOGTest, TypedMessageType) {
  LOG(SomeModule, 0);
  VerifyType<SomeModule>();
}

TEST_F(LOGTest, TypedMessageMessage) {
  LOG(SomeModule, 0) << kMessage;
  VerifyMessage(kMessage);
}

TEST_F(LOGTest, VerbosityNotLowEnough) {
  SetVerbosity<SomeModule>(1);
  LOG(SomeModule, 2) << kMessage;
  VerifyEmpty();
}

TEST_F(LOGTest, VerbosityLowEnough) {
  SetVerbosity<SomeModule>(1);
  LOG(SomeModule, 1);
  VerifyType<SomeModule>();
}

TEST_F(LOGTest, MessageOfAnotherType) {
  SetVerbosity<SomeModule>(1);
  LOG(AnotherModule, 1) << kMessage;
  VerifyEmpty();
}


TEST_F(LOGTest, PrintSignedChar) {
  const signed char value = 65;
  LOG() << value;
  VerifyMessage("65");
}

TEST_F(LOGTest, PrintUnsignedChar) {
  const unsigned char value = 65;
  LOG() << value;
  VerifyMessage("65");
}

TEST_F(LOGTest, PrintChar) {
  LOG() << 'A';
  VerifyMessage("A");
}

}  // namespace LOG
