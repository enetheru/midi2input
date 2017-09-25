// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_MUTEX_WIN32_H_
#define ELOG_MUTEX_WIN32_H_

#include <windows.h>

namespace LOG {

class Mutex {
 public:
  Mutex() {
    InitializeCriticalSection(&critical_section_);
  }

  ~Mutex() {
    DeleteCriticalSection(&critical_section_);
  }

  void Lock() {
    EnterCriticalSection(&critical_section_);
  }

  void Unlock() {
    LeaveCriticalSection(&critical_section_);
  }

 private:
  CRITICAL_SECTION critical_section_;
};

}  // namespace LOG

#endif  // ELOG_MUTEX_WIN32_H_
