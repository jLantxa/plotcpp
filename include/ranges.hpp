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

#ifndef _PLOTCPP_INCLUDE_RANGES_HPP_
#define _PLOTCPP_INCLUDE_RANGES_HPP_

#include <set>

#include "plotcpp.hpp"

namespace plotcpp {
namespace ranges {

using Range = std::pair<Real, Real>;

/**
 * \brief Partitions a range into a number of intermediate values
 * from the minumum value to the maximum value.
 */
std::set<Real> TrivialPartitionRange(const Range& range,
                                     unsigned int num_markers);

/**
 * \brief Partitions a range into a number of intermediate values
 * showing relevant values.
 */
std::set<Real> PartitionRange(const Range& range, unsigned int num_markers);

}  // namespace ranges
}  // namespace plotcpp

#endif  // _PLOTCPP_INCLUDE_RANGES_HPP_
