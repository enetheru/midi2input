// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_BENCHMARK_H_
#define ELOG_BENCHMARK_H_

#include "elog.h"
#include "scoped_benchmark.h"
#include "typed_benchmark.h"

#define BENCHMARK(...) ELOG_I_OVERLOAD(ELOG_I_BENCHMARK_, __VA_ARGS__)

#define ELOG_I_BENCHMARK_1(name) \
  if (::LOG::ScopedBenchmark< ::LOG::INFO> name = \
      ::LOG::ScopedBenchmark< ::LOG::INFO>(ELOG_I_STRINGIZE(name), \
                                           ELOG_I_FILE, ELOG_I_LINE)); else

#define ELOG_I_BENCHMARK_2(suite, name) \
  if (::LOG::ScopedBenchmark< ::LOG::INFO> name = \
      ::LOG::ScopedBenchmark< ::LOG::INFO>(ELOG_I_STRINGIZE(name), \
                                           ELOG_I_FILE, ELOG_I_LINE, \
                                           &suite)); else

#define ELOG_I_BENCHMARK_3(type, verbosity, name) \
  if (::LOG::TypedBenchmark name = \
      ::LOG::TypedBenchmark(ELOG_I_STRINGIZE(name), \
                            ::LOG::TypeInfo(::LOG::Type<type>()), \
                            ELOG_I_FILE, ELOG_I_LINE, verbosity)); else

#define ELOG_I_BENCHMARK_4(suite, type, verbosity, name) \
  if (::LOG::TypedBenchmark name = \
      ::LOG::TypedBenchmark(ELOG_I_STRINGIZE(name), \
                            ::LOG::TypeInfo(::LOG::Type<type>()), \
                            ELOG_I_FILE, ELOG_I_LINE, verbosity, &suite)); else

#endif  // ELOG_BENCHMARK_H_
