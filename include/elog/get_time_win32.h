// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_GET_TIME_WIN32_H_
#define ELOG_GET_TIME_WIN32_H_

#include <windows.h>

namespace LOG {

inline double GetTimeSec() {
  LARGE_INTEGER t, f;
  QueryPerformanceCounter(&t);
  QueryPerformanceFrequency(&f);
  return t.QuadPart * 1.0 / f.QuadPart;
}

}  // namespace LOG

#endif  // ELOG_GET_TIME_WIN32_H_
