// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_MUTEX_H_
#define ELOG_MUTEX_H_

#ifdef _WIN32
# include "mutex_win32.h"
#else
# include "mutex_posix.h"
#endif

namespace LOG {

class MutexLock {
 public:
  explicit MutexLock(Mutex& mutex)
      : mutex_(mutex) {
    mutex.Lock();
  }

  ~MutexLock() {
    mutex_.Unlock();
  }

 private:
  Mutex& mutex_;
};

}  // namespace LOG

#endif  // ELOG_MUTEX_H_
