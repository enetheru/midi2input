// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_DEMANGLE_GXX_H_
#define ELOG_DEMANGLE_GXX_H_

#include "config.h"
#ifdef ELOG_I_USE_TR1_HEADER
# include <tr1/memory>
#else
# include <memory>
#endif
#include <cxxabi.h>

namespace LOG {

class Demangle {
 public:
  explicit Demangle(const char* mangled_name) {
    int status;
    char* demangled_name = abi::__cxa_demangle(mangled_name, 0, 0, &status);
    type_name_.reset(demangled_name);
  }

  const char* GetName() const {
    return type_name_.get();
  }

 private:
  std::tr1::shared_ptr<char> type_name_;
};

}  // namespace LOG

#endif  // ELOG_DEMANGLE_GXX_H_
