// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_MUTEX_POSIX_H_
#define ELOG_MUTEX_POSIX_H_

#include <pthread.h>

namespace LOG {

class Mutex {
 public:
  Mutex() {
    pthread_mutex_init(&mutex_, 0);
  }

  ~Mutex() {
    pthread_mutex_destroy(&mutex_);
  }

  void Lock() {
    pthread_mutex_lock(&mutex_);
  }

  void Unlock() {
    pthread_mutex_unlock(&mutex_);
  }

 private:
  pthread_mutex_t mutex_;
};

}  // namespace LOG

#endif  // ELOG_MUTEX_POSIX_H_
