// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_ELOG_H_
#define ELOG_ELOG_H_

#include "general_log.h"
#include "typed_log.h"

namespace LOG {

struct NullStream {
  template <typename T>
  NullStream& operator<<(T) const {
    return *this;
  }
};

struct LogEmitTrigger {
  template <typename T>
  void operator&(const T& t) const {
    t.PushMessage();
  }
};

struct VoidEmitter {
  void operator&(NullStream) const {}
};

}  // namespace LOG

#ifdef _MSC_VER

# define ELOG_I_FILE __FILE__
# define ELOG_I_LINE __COUNTER__

# define ELOG_I_CAT(x, y) ELOG_I_CAT_I ELOG_I_EXPAND((x, y))
# define ELOG_I_EXPAND(x) ELOG_I_EXPAND_I(x)
# define ELOG_I_EXPAND_I(x) x
# define ELOG_I_CAT_I(x, y) x ## y

# define ELOG_I_LPAR (
# define ELOG_I_RPAR )

#else  // ifndef _MSC_VER

# define ELOG_I_FILE __FILE__
# define ELOG_I_LINE __LINE__

# define ELOG_I_CAT(x, y) ELOG_I_CAT_I(x, y)
# define ELOG_I_CAT_I(x, y) x ## y

#endif  // _MSC_VER


#define ELOG_I_STRINGIZE(x) ELOG_I_STRINGIZE_I(x)
#define ELOG_I_STRINGIZE_I(x) #x

// Length of tuple
#ifdef _MSC_VER
# define ELOG_I_TUPLE_LEN(...) \
  ELOG_I_TUPLE_LEN_I ELOG_I_LPAR _, __VA_ARGS__, 5, 4, 3, 2, 1, 0, _ ELOG_I_RPAR
#else
# define ELOG_I_TUPLE_LEN(...) \
  ELOG_I_TUPLE_LEN_I(_, ##__VA_ARGS__, 5, 4, 3, 2, 1, 0, _)
#endif

#define ELOG_I_TUPLE_LEN_I(_0, _1, _2, _3, _4, _5, n, ...) n

// Macro overload
#define ELOG_I_OVERLOAD(name, ...) \
  ELOG_I_CAT(name, ELOG_I_TUPLE_LEN(__VA_ARGS__))(__VA_ARGS__)


#define LOG(...) ELOG_I_OVERLOAD(ELOG_I_LOG_, __VA_ARGS__)

#define ELOG_I_LOG_0() ELOG_I_LOG_1(INFO)

// '<' and '::' must be separated by white space, because <:: is a trigram
#define ELOG_I_LOG_1(level) \
  ::LOG::LogEmitTrigger() & \
  ::LOG::GeneralLog< ::LOG::level>(ELOG_I_FILE, ELOG_I_LINE).GetReference()

#define ELOG_I_LOG_2(type, verbosity) \
  ::LOG::LogEmitTrigger() & \
  ::LOG::TypedLog(::LOG::TypeInfo(::LOG::Type<type>()), (verbosity), \
                  ELOG_I_FILE, ELOG_I_LINE).GetReference()

// ditto
#define CHECK(cond) \
  (cond) ? (void)0 : ::LOG::LogEmitTrigger() & \
  ::LOG::GeneralLog< ::LOG::CHECK>(ELOG_I_FILE, ELOG_I_LINE).GetReference()


#ifdef NDEBUG
# define ELOG_I_NULL_STREAM \
  true ? (void)0 : ::LOG::VoidEmitter() & ::LOG::NullStream()
# define DLOG(...) ELOG_I_NULL_STREAM
# define DCHECK(...) ELOG_I_NULL_STREAM
#else
# define DLOG LOG
# define DCHECK CHECK
#endif

#endif  // ELOG_ELOG_H_
