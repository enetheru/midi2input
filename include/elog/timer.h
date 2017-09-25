// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_TIMER_H_
#define ELOG_TIMER_H_

#ifdef _WIN32
# include "get_time_win.h"
#else
# include "get_time_posix.h"
#endif

namespace LOG {

class Timer {
 public:
  Timer()
      : start_sec_(GetTimeSec()) {
  }

  double GetTime() const {
    const double now = GetTimeSec();
    return now - start_sec_;
  }

 private:
  double start_sec_;
};

}  // namespace LOG

#endif  // ELOG_TIMER_H_
