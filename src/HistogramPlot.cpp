/*
 * plotcpp is a 2D plotting library for modern C++
 *
 * Copyright 2022  Javier Lancha Vázquez <javier.lancha@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "HistogramPlot.hpp"

#include <algorithm>
#include <vector>

#include "utility.hpp"

namespace plotcpp {

HistogramPlot::HistogramPlot() { SetBarRelativeWidth(1.0f); }

void HistogramPlot::Clear() { ClearData(); }

void HistogramPlot::Plot(const std::vector<Real>& values, unsigned int num_bins,
                         const Color& color) {
  ClearData();

  std::vector<Real> intervals = CalculateIntervals(values, num_bins);
  std::vector<Real> bins = CalculateBins(intervals);
  std::vector<Real> counts = CalculateHistogram(values, intervals);
  m_num_bars = counts.size();

  m_numeric_x_data = bins;
  m_y_data.push_back(DataSeries{counts, color});
}

std::vector<Real> HistogramPlot::CalculateIntervals(
    const std::vector<Real>& values, unsigned int num_bins) {
  std::vector<Real> intervals;
  intervals.resize(num_bins + 1);

  const auto [min, max] =
      std::pair<Real, Real>{*std::min_element(values.begin(), values.end()),
                            *std::max_element(values.begin(), values.end())};

  if (min == max) {
    return {min};
  }

  const Real interval = (max - min) / num_bins;
  for (unsigned int i = 0; (i < num_bins + 1); ++i) {
    intervals[i] = min + static_cast<float>(i) * interval;
  }

  return intervals;
}

std::vector<Real> HistogramPlot::CalculateBins(
    const std::vector<Real>& intervals) {
  const std::size_t num_intervals = intervals.size();
  if (num_intervals == 0) {
    return {};
  } else if (num_intervals == 1) {
    return {intervals.front()};
  }

  const std::size_t num_bins = num_intervals - 1;
  std::vector<Real> bins;
  bins.resize(num_bins);

  for (std::size_t i = 0; i < num_bins; ++i) {
    bins[i] = (intervals[i + 1] + intervals[i]) / 2.0f;
  }

  return bins;
}

std::vector<Real> HistogramPlot::CalculateHistogram(
    const std::vector<Real>& values, const std::vector<Real>& intervals) {
  if (intervals.size() == 0) {
    return {};
  }

  const std::size_t num_bins = intervals.size() - 1;
  std::vector<std::size_t> counts(num_bins, 0);

  for (const auto& value : values) {
    const auto [index, found] = BinarySearchInterval(value, intervals);
    if (found) {
      ++counts[index];
    }
  }

  return adaptor::Real(counts);
}

}  // namespace plotcpp
