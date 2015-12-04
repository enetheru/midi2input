// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_SAFE_BOOL_H_
#define ELOG_SAFE_BOOL_H_

#include <cstddef>

namespace LOG {

struct SafeBoolBase {
  typedef void (SafeBoolBase::*BoolType)() const;
  void True() const {}
};

template <typename T>
class SafeBool : SafeBoolBase {
 public:
  operator BoolType() const {
    return static_cast<const T*>(this)->BoolTest() ?
        &SafeBoolBase::True : NULL;
  }
};

}  // namespace LOG

#endif  // ELOG_SAFE_BOOL_H_

