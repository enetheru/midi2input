// Copyright (c) 2011 Seiya Tokui <beam.web@gmail.com>. All Rights Reserved.
// This source code is distributed under MIT License in LICENSE file.

#ifndef ELOG_BENCHMARK_SUITE_H_
#define ELOG_BENCHMARK_SUITE_H_

#include <cstddef>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "logger.h"
#include "logger_factory.h"
#include "mutex.h"
#include "timer.h"

namespace LOG {

class BenchmarkSuite {
 public:
  typedef std::vector<std::pair<std::string, double> > Chart;

  static const int kDefaultPrecision = 5;

  explicit BenchmarkSuite(const std::string& title)
      : title_(title),
        precision_(kDefaultPrecision) {
  }

  const std::string& title() const {
    return title_;
  }

  void set_precision(int precision) {
    precision_ = precision;
  }

  void AddCase(const std::string& case_name, double time) {
    MutexLock lock(chart_mutex_);
            chart_.push_back(std::make_pair(case_name, time));
  }

  void LogChart(LogLevel level = INFO, Logger* logger = NULL) const {
    if (!logger) {
      logger = &GetLogger();
    }
    const std::string chart_string = PrintChart();
    logger->PushRawMessage(level, chart_string);
  }

  std::string PrintChart() const {
    static const char kTimeColumnTitle[] = "time (sec)";
    MutexLock lock(chart_mutex_);

    const std::size_t title_width = GetMaxTitleLength();
    const std::size_t time_column_width =
        std::max(GetTimeColumnWidth(), sizeof(kTimeColumnTitle) - 1);

    std::ostringstream stream;

    PrintTopRow(title_width, kTimeColumnTitle, stream);
    PrintHorizontalSeparator(title_width, time_column_width, stream);
    PrintAllCasesResult(title_width, time_column_width, stream);
    PrintHorizontalSeparator(title_width, time_column_width, stream);
    PrintStatistics(title_width, time_column_width, stream);

    stream << std::flush;

    return stream.str();
  }

 private:
  static std::size_t GetDecimalPointIndex(double x) {
    std::size_t position = 1;
    while (x >= 10) {
      x /= 10;
      ++position;
    }
    return position;
  }

  void PrintTopRow(std::size_t title_width,
                   const char* time_column_title,
                   std::ostream& os) const {
    os << std::left << std::setw(title_width) << title_ << " | "
       << time_column_title << '\n';
  }

  void PrintHorizontalSeparator(std::size_t title_width,
                                std::size_t time_column_width,
                                std::ostream& os) const {
    os << std::setw(title_width) << std::setfill('-') << "" << "-+-"
       << std::setw(time_column_width) << std::setfill('-') << "" << '\n';
  }

  void PrintAllCasesResult(std::size_t title_width,
                           std::size_t time_column_width,
                           std::ostream& os) const {
    for (std::size_t i = 0; i < chart_.size(); ++i) {
      PrintCaseResult(chart_[i].first, title_width,
                      chart_[i].second, time_column_width, os);
    }
  }

  void PrintCaseResult(const std::string& case_title,
                       std::size_t title_width,
                       double time,
                       std::size_t time_column_width,
                       std::ostream& os) const {
    SetResultFormat(os);
    os << std::left << std::setw(title_width) << case_title << " | "
       << std::right << std::setw(time_column_width) << time << '\n';
  }

  void PrintStatistics(std::size_t title_width,
                       std::size_t time_column_width,
                       std::ostream& os) const {
    const double time_sum = GetTimeSum();
    const double total = timer_.GetTime();
    const double other = total - time_sum;

    PrintCaseResult("(other)", title_width, other, time_column_width, os);
    PrintCaseResult("total", title_width, total, time_column_width, os);
  }

  void SetResultFormat(std::ostream& os) const {
    os << std::setfill(' ') << std::fixed << std::setprecision(precision_);
  }

  std::size_t GetTimeColumnWidth() const {
    std::size_t decimal_point_index = 1;
    for (std::size_t i = 0; i < chart_.size(); ++i) {
      decimal_point_index = std::max(decimal_point_index,
                                     GetDecimalPointIndex(chart_[i].second));
    }
    return decimal_point_index + precision_ + 1;
  }

  std::size_t GetMaxTitleLength() const {
    static const std::size_t kLeftColumnMinSize = 7;
    std::size_t length = std::max(kLeftColumnMinSize, title_.size());
    for (std::size_t i = 0; i < chart_.size(); ++i) {
      length = std::max(length, chart_[i].first.size());
    }
    return length;
  }

  double GetTimeSum() const {
    double sum = 0.0;
    for (std::size_t i = 0; i < chart_.size(); ++i) {
      sum += chart_[i].second;
    }
    return sum;
  }

  mutable Mutex chart_mutex_;
  Chart chart_;
  Timer timer_;
  std::string title_;
  int precision_;
};

}  // namespace LOG

#endif  // ELOG_BENCHMARK_SUITE_H_
