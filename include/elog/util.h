// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_UTIL_H_
#define ELOG_UTIL_H_

namespace LOG {

enum AvoidODR {
  AVOID_ODR
};

struct Call {
  template <typename Function>
  void operator()(Function function) const {
    function();
  }
};

struct EmptyFunction {
  template <typename T>
  void operator()(T) const {
  }
};

class Noncopyable {
 protected:
  Noncopyable() {}
  ~Noncopyable() {}

 private:
  Noncopyable(const Noncopyable&);  // no implementation
  Noncopyable& operator=(const Noncopyable&);  // no implementation
};

template <typename T>
void CheckedDelete(T* p) {
  typedef char T_must_be_complete[sizeof(T) ? 1 : -1];
  (void) sizeof(T_must_be_complete);
  delete p;
}


typedef char TrueType;
typedef struct { char a[2]; } FalseType;

template <typename T>
class IsContainer {
 private:
  template <typename Type>
  static TrueType HasConstIterator(typename Type::value_type*);

  template <typename Type>
  static FalseType HasConstIterator(...);

 public:
  static const bool value =
      sizeof(HasConstIterator<T>(0)) == sizeof(TrueType);
};

}  // namespace LOG

// #include <vector>
// static int v[LOG::IsContainer<std::vector<int> >::value ? 1 : -1];
// static int w[LOG::IsContainer<char>::value ? -1 : 1];

#endif  // ELOG_UTIL_H_
