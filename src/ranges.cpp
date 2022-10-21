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
#include <vector>

#include "plotcpp.hpp"

namespace plotcpp {
namespace ranges {

std::vector<Real> TrivialPartitionRange(const Range& range,
                                        unsigned int num_markers) {
  std::vector<Real> values;

  const Real min = std::min(range.first, range.second);
  const Real max = std::max(range.first, range.second);
  const Real interval = (max - min) / (num_markers - 1);

  for (Real marker = min; marker <= max; marker += interval) {
    values.push_back(marker);
  }

  return values;
}

std::vector<Real> PartitionRange(const Range& range, unsigned int num_markers) {
  // TODO: Implement algorithm
  return TrivialPartitionRange(range, num_markers);
}

}  // namespace ranges
}  // namespace plotcpp
