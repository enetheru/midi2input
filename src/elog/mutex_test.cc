// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#include "config.h"

#include <vector>
#ifdef ELOG_I_USE_TR1_HEADER
# include <tr1/functional>
#else
# include <functional>
#endif
#include <gtest/gtest.h>
#include "mutex.h"
#include "thread.h"

namespace LOG {

namespace {

class MutexTester {
 public:
  MutexTester()
      : start_flag_A_(false),
        start_flag_B_(false),
        end_flag_A_(false),
        end_flag_B_(false) {
  }

  void Run() {
    std::tr1::function<void ()> thread_body_a =
        std::tr1::bind(&MutexTester::ThreadA, this);
    std::tr1::function<void ()> thread_body_b =
        std::tr1::bind(&MutexTester::ThreadB, this);

    Thread thread_a(thread_body_a);
    Thread thread_b(thread_body_b);

    thread_b.Run();
    thread_a.Run();

    while (!end_flag_A_ || !end_flag_B_) {
      continue;
    }
  }

  void ThreadA() {
    {
      MutexLock lock(mutex1_);
      start_flag_A_ = true;
      while (!start_flag_B_) continue;
      items_.push_back(0);
    }
    {
      MutexLock lock(mutex2_);
      items_.push_back(3);
    }
    end_flag_A_ = true;
  }

  void ThreadB() {
    {
      MutexLock lock2(mutex2_);
      start_flag_B_ = true;
      while (!start_flag_A_) continue;
      {
        MutexLock lock1(mutex1_);
        items_.push_back(1);
      }
      items_.push_back(2);
    }
    end_flag_B_ = true;
  }

  void Verify() const {
    for (size_t i = 0; i < items_.size(); ++i) {
      EXPECT_EQ(i, items_[i]);
    }
  }

 private:
  std::vector<int> items_;
  Mutex mutex1_;
  Mutex mutex2_;
  volatile bool start_flag_A_;
  volatile bool start_flag_B_;
  volatile bool end_flag_A_;
  volatile bool end_flag_B_;
};

}  // anonymous namespace

TEST(MutexTest, MutexLock) {
  MutexTester tester;
  tester.Run();
  tester.Verify();
}

}  // namespace LOG
