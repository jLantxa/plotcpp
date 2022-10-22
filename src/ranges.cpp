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

#include "ranges.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <set>

#include "plotcpp.hpp"

namespace plotcpp {
namespace ranges {

static const auto less = [](Real a, Real b) { return a < b; };
static const auto greater = [](Real a, Real b) { return a > b; };

std::vector<Real> Range(Real start, Real end, Real step) {
  const bool wrong_direction =
      ((start < end) && (step <= 0)) || ((start > end) && (step >= 0));
  if (wrong_direction) {
    return {};
  }

  std::function<bool(Real, Real)> cmp;
  if (start <= end) {
    cmp = less;
  } else {
    cmp = greater;
  }

  std::vector<Real> range;
  range.reserve(1 + static_cast<std::size_t>((end - start) / step));

  while (cmp(start, end)) {
    range.push_back(start);
    start += step;
  }

  // Add end
  if (range.back() != end) {
    range.push_back(end);
  }

  return range;
}

std::set<Real> TrivialPartitionRange(const Interval& range,
                                     unsigned int num_markers) {
  std::set<Real> values;

  const Real min = std::min(range.first, range.second);
  const Real max = std::max(range.first, range.second);
  const Real interval = (max - min) / (num_markers - 1);

  for (Real marker = min; marker <= max; marker += interval) {
    values.insert(marker);
  }

  return values;
}

std::set<Real> PartitionRange(const Interval& range, unsigned int num_markers) {
  // TODO: Implement algorithm
  return TrivialPartitionRange(range, num_markers);
}

}  // namespace ranges
}  // namespace plotcpp
