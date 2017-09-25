// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_DEMANGLE_MSVC_H_
#define ELOG_DEMANGLE_MSVC_H_

namespace LOG {

class Demangle {
 public:
  explicit Demangle(const char* name)
      : name_(name) {
  }

  const char* GetName() const {
    return name_;
  }

 private:
  const char* name_;
};

}  // namespace LOG

#endif  // ELOG_DEMANGLE_MSVC_H_
