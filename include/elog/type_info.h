// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_TYPE_INFO_H_
#define ELOG_TYPE_INFO_H_

#include "config.h"

#include <typeinfo>
#ifdef ELOG_I_USE_TR1_HEADER
# include <tr1/functional>
#else
# include <functional>
#endif
#include "demangle.h"

namespace LOG {

template <typename T> class Type {};

// Type name of T.
// For each type T, the value of GlobalTypeName<T>::name as a pointer is
// unique for all compilation units, even if addresses of typeid(T) are
// different.
template <typename T>
struct GlobalTypeName {
  static const char* name;
};

template <typename T>
const char* GlobalTypeName<T>::name = typeid(T).name();

class TypeInfo {
 public:
  struct Hash : std::unary_function<TypeInfo, std::size_t> {
    std::size_t operator()(TypeInfo type_info) const {
      return std::tr1::hash<const char*>()(type_info.global_type_name_);
    }
  };

  template <typename T>
  explicit TypeInfo(Type<T>)
      : global_type_name_(GlobalTypeName<T>::name) {
  }

  Demangle GetTypeName() const {
    return Demangle(global_type_name_);
  }

 private:
#define ELOG_I_DEFINE_TYPEINFO_OPERATOR(op) \
  friend bool operator op(const TypeInfo& lhs, const TypeInfo& rhs) { \
    return lhs.global_type_name_ op rhs.global_type_name_; \
  }

  ELOG_I_DEFINE_TYPEINFO_OPERATOR(==)
  ELOG_I_DEFINE_TYPEINFO_OPERATOR(!=)
  ELOG_I_DEFINE_TYPEINFO_OPERATOR(<)
  ELOG_I_DEFINE_TYPEINFO_OPERATOR(<=)
  ELOG_I_DEFINE_TYPEINFO_OPERATOR(>=)
  ELOG_I_DEFINE_TYPEINFO_OPERATOR(>)

#undef ELOG_I_DEFINE_TYPEINFO_OPERATOR

  const char* global_type_name_;
};

}  // namespace LOG

#endif  // ELOG_TYPE_INFO_H_
