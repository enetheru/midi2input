// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_TYPED_BENCHMARK_H_
#define ELOG_TYPED_BENCHMARK_H_

#include <string>
#include "benchmark_suite.h"
#include "elog.h"
#include "safe_bool.h"
#include "timer.h"
#include "type_info.h"
#include "typed_log.h"

namespace LOG {

class TypedBenchmark : public SafeBool<TypedBenchmark> {
 public:
  TypedBenchmark(const std::string& title,
                 TypeInfo type_info,
                 const char* source_file_name,
                 int line_number,
                 int verbosity = 0,
                 BenchmarkSuite* suite = NULL,
                 Logger* logger = NULL)
      : typed_log_(type_info, verbosity, source_file_name, line_number, logger),
        title_(title),
        suite_(suite),
        done_(false) {
    TypedLog start_log(type_info, verbosity,
                       source_file_name, line_number, logger);
    start_log << title << ": start...";
    start_log.PushMessage();
  }

  TypedBenchmark(const TypedBenchmark& typed_benchmark)
      : typed_log_(typed_benchmark.typed_log_),
        title_(typed_benchmark.title_),
        suite_(typed_benchmark.suite_),
        done_(typed_benchmark.done_) {
  }

  ~TypedBenchmark() {
    Print();
  }

  bool BoolTest() const {
    return false;
  }

  double GetTime() const {
    return timer_.GetTime();
  }

  void Print() {
    if (done_) return;
    done_ = true;

    PrintWithoutCheck();
  }

 private:
  void PrintWithoutCheck() {
    const double time = timer_.GetTime();
    if (suite_) {
      suite_->AddCase(title_, time);
    }
    typed_log_ << title_ << ": " << time << " sec";
    typed_log_.PushMessage();
  }

  TypedLog typed_log_;
  std::string title_;
  Timer timer_;
  BenchmarkSuite* suite_;

  bool done_;
};

}  // namespace LOG

#endif  // ELOG_TYPED_BENCHMARK_H_
