// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_THREAD_POSIX_H_
#define ELOG_THREAD_POSIX_H_

#include "config.h"

#ifdef ELOG_I_USE_TR1_HEADER
# include <tr1/functional>
#else
# include <functional>
#endif
#include <pthread.h>
#include "util.h"

namespace LOG {

class Thread : Noncopyable {
 public:
  Thread() : is_running_(false) {
  }

  template <typename ThreadBody>
  explicit Thread(ThreadBody thread_body)
      : thread_body_(thread_body),
        is_running_(false) {
  }

  ~Thread() {
    Detach();
  }

  template <typename ThreadBody>
  void set_thread_body(ThreadBody thread_body) {
    thread_body_ = thread_body;
  }

  void Run() {
    pthread_create(&thread_handle_, NULL, StaticThreadBody, this);
  }

  void Detach() {
    if (is_running_) {
      pthread_detach(thread_handle_);
    }
    is_running_ = false;
  }

  void Join() {
    pthread_join(thread_handle_, NULL);
  }

 private:
  static void* StaticThreadBody(void* self_ptr) {
    Thread& self = *reinterpret_cast<Thread*>(self_ptr);
    self.is_running_ = true;
    self.thread_body_();
    self.is_running_ = false;
    return NULL;
  }

  pthread_t thread_handle_;
  std::tr1::function<void ()> thread_body_;
  volatile bool is_running_;
};

}  // namespace LOG

#endif  // ELOG_THREAD_POSIX_H_
