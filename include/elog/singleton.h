// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_SINGLETON_H_
#define ELOG_SINGLETON_H_

#include <cstdlib>
#include <algorithm>
#include <vector>
#include "atomic.h"
#include "mutex.h"
#include "util.h"

namespace LOG {

template <typename T> class Singleton;

template <AvoidODR>
class SingletonFinalizerAvoidODR {
 public:
  typedef void (* Finalizer)();

  static void RegisterFinalizer(Finalizer finalizer) {
    MutexLock lock(mutex_);
    if (!registered_) {
      std::atexit(Finalize);
      registered_ = true;
    }
    finalizers_.push_back(finalizer);
  }

 private:
  static void Finalize() {
    Unregister();

    while (!finalizers_.empty()) {
      const std::vector<Finalizer> finalizers = GetCurrentFinalizers();
      std::for_each(finalizers.begin(), finalizers.end(), Call());
    }
  }

  static std::vector<void (*)()> GetCurrentFinalizers() {
    std::vector<Finalizer> current_finalizers;
    current_finalizers.swap(finalizers_);
    return current_finalizers;
  }

  static void Unregister() {
    MutexLock lock(mutex_);
    registered_ = false;
  }

  static std::vector<Finalizer> finalizers_;
  static Mutex mutex_;
  static bool registered_;
};

template <AvoidODR N>
std::vector<typename SingletonFinalizerAvoidODR<N>::Finalizer>
SingletonFinalizerAvoidODR<N>::finalizers_;

template <AvoidODR N>
Mutex SingletonFinalizerAvoidODR<N>::mutex_;

template <AvoidODR N>
bool SingletonFinalizerAvoidODR<N>::registered_;

typedef SingletonFinalizerAvoidODR<AVOID_ODR> SingletonFinalizer;

template <typename T>
class Singleton : Noncopyable {
 public:
  static T& Get() {
    CallOnce(initialization_flag_, Init());
    if (!instance_) {
      // This code is executed only after Finalize() has been called, and
      // Finalize() is called only after main() ended, so we do not care about
      // thread safety.
      // NOTE: Be careful to stack overflow. If instance_->~T() always calls
      // Singleton<T>::Get(), then it tries to create a new instance of T, and
      // instance_->~T() will be called at exit again.
      Init()();
    }
    return *instance_;
  }

 protected:
  Singleton() {}

 private:
  struct Init {
    void operator()() const {
      SingletonFinalizer::RegisterFinalizer(Finalize);
      instance_ = new T;
    }
  };

  static void Finalize() {
    T* instance = instance_;
    instance_ = NULL;

    // We cannot use CheckedDelete, because T::~T can be private and
    // Singleton<T> can be a friend class of T.
    typedef char T_must_be_complete[sizeof(T) ? 1 : -1];
    (void) sizeof(T_must_be_complete);
    delete instance;
  }

  static OnceFlag initialization_flag_;
  static T* instance_;
};

template <typename T>
OnceFlag Singleton<T>::initialization_flag_;

template <typename T>
T* Singleton<T>::instance_;

}  // namespace LOG

#endif  // ELOG_SINGLETON_H_
