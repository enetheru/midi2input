// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_GET_TIME_POSIX_H_
#define ELOG_GET_TIME_POSIX_H_

#include <sys/time.h>

namespace LOG {

inline double GetTimeSec() {
  timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

}  // namespace LOG

#endif  // ELOG_GET_TIME_POSIX_H_
