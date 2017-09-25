// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_PUT_AS_STRING_H_
#define ELOG_PUT_AS_STRING_H_

#include <cstddef>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "util.h"

namespace LOG {

template <typename T, bool IsContainer = IsContainer<T>::value>
struct StringBuildFunction {
  template <typename Stream>
  void operator()(const T& t, Stream& stream) const {
    stream << t;
  }
};

template <typename S, typename T>
struct StringBuildFunction<std::pair<S, T>, false> {
  template <typename Stream>
  void operator()(const std::pair<S, T>& t, Stream& stream) const {
    stream << '(';
    PutAsString(t.first, stream);
    stream << ',';
    PutAsString(t.second, stream);
    stream << ')';
  }
};

template <typename Container>
struct StringBuildFunction<Container, true> {
  template <typename Stream>
  void operator()(const Container& container, Stream& stream) const {
    typedef typename Container::const_iterator Iterator;
    Iterator begin = container.begin();
    for (Iterator itr = container.begin(); itr != container.end(); ++itr) {
      if (itr != begin) {
        stream << ',';
      }
      PutAsString(*itr, stream);
    }
  }
};

template <>
struct StringBuildFunction<std::string, true> {
  template <typename Stream>
  void operator()(const std::string& string, Stream& stream) const {
    stream << string;
  }
};

template <>
struct StringBuildFunction<signed char, false> {
  template <typename Stream>
  void operator()(signed char signed_char, Stream& stream) const {
    stream << static_cast<int>(signed_char);
  }
};

template <>
struct StringBuildFunction<unsigned char, false> {
  template <typename Stream>
  void operator()(unsigned char unsigned_char, Stream& stream) const {
    stream << static_cast<unsigned int>(unsigned_char);
  }
};


template <typename T, typename Stream>
inline void PutAsString(const T& t, Stream& stream) {
  StringBuildFunction<T>()(t, stream);
}

}  // namespace LOG

#endif  // ELOG_PUT_AS_STRING_H_
