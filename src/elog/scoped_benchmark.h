// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_SCOPED_BENCHMARK_H_
#define ELOG_SCOPED_BENCHMARK_H_

#include <string>
#include "benchmark_suite.h"
#include "elog.h"
#include "general_log.h"
#include "safe_bool.h"
#include "timer.h"

namespace LOG {

template <LogLevel LEVEL>
class ScopedBenchmark : public SafeBool<ScopedBenchmark<LEVEL> > {
 public:
  ScopedBenchmark(const std::string& title,
                  const char* source_file_name,
                  int line_number,
                  BenchmarkSuite* suite = NULL,
                  Logger* logger = NULL)
      : general_log_(source_file_name, line_number, logger),
        title_(title),
        suite_(suite),
        done_(false) {
    GeneralLog<LEVEL> start_log(source_file_name, line_number, logger);
    start_log << title << ": start...";
    start_log.PushMessage();
  }

  ScopedBenchmark(const ScopedBenchmark& scoped_benchmark)
      : general_log_(scoped_benchmark.general_log_),
        title_(scoped_benchmark.title_),
        suite_(scoped_benchmark.suite_),
        done_(scoped_benchmark.done_) {
  }

  ~ScopedBenchmark() {
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
    general_log_ << title_ << ": " << time << " sec";
    general_log_.PushMessage();
  }

  GeneralLog<LEVEL> general_log_;
  std::string title_;
  Timer timer_;
  BenchmarkSuite* suite_;

  bool done_;
};

}  // namespace LOG

#endif  // ELOG_SCOPED_BENCHMARK_H_
