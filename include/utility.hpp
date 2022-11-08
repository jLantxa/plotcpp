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

#ifndef _PLOTCPP_INCLUDE_UTILITY_HPP_
#define _PLOTCPP_INCLUDE_UTILITY_HPP_

#include <cmath>
#include <cstdint>
#include <functional>
#include <set>
#include <vector>

namespace plotcpp {

/** Internal real number type */
using Real = double;

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

namespace adaptor {
/**
 * \brief Convert any numeric type to the internal real representation
 *
 * \tparam T Custom numeric type
 * \param x Value
 * \return Real Conversion from T to Real
 */
template <typename T>
Real Real(T x) {
  return static_cast<::plotcpp::Real>(x);
}

/**
 * \brief Convert a vector of any numeric type to the internal real
 * representation
 *
 * \tparam T Custom numeric type
 * \param v Vector
 * \return std::vector<Real> Conversion from T to Real
 */
template <typename T>
std::vector<::plotcpp::Real> Real(std::vector<T> v) {
  const std::size_t size = v.size();

  std::vector<::plotcpp::Real> real_vector;
  real_vector.resize(size);

  for (std::size_t i = 0; i < size; ++i) {
    real_vector[i] = Real(v[i]);
  }

  return real_vector;
}

}  // namespace adaptor

namespace ranges {

template <typename T>
using Interval = std::pair<T, T>;

/**
 * \brief Returns a vector of Real numbers from a to b in equally spaced
 * intervals except, maybe, the last interval.
 *
 * \tparam T Custom numeric type
 */
template <typename T>
std::vector<T> MakeRange(T start, T end, T step) {
  const bool wrong_direction =
      ((start < end) && (step <= 0)) || ((start > end) && (step >= 0));
  if (wrong_direction) {
    return {};
  }

  static const auto less = [](T a, T b) { return a < b; };
  static const auto greater = [](T a, T b) { return a > b; };
  std::function<bool(T, T)> cmp;
  if (start <= end) {
    cmp = less;
  } else {
    cmp = greater;
  }

  std::vector<T> range;
  range.reserve(
      1 + static_cast<std::size_t>(std::ceil(std::abs((end - start) / step))));

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

/**
 * \brief Generates a vector of Real y as a function of a vector of Real x.
 *
 * \param x Input vector
 * \param function A function f such that y=f(x)
 * \return A vector y such that y=function(x)
 */
template <typename T>
std::vector<T> Generate(const std::vector<T>& x,
                        const std::function<T(T)>& function) {
  const std::size_t size = x.size();
  if (size == 0) {
    return {};
  }

  std::vector<T> y;
  y.resize(size);

  for (std::size_t i = 0; i < size; ++i) {
    y[i] = function(x[i]);
  }

  return y;
}

/**
 * \brief Partitions a range into a number of intermediate values
 * from the minumum value to the maximum value.
 */
template <typename T>
std::set<T> TrivialPartitionRange(const Interval<T>& range,
                                  unsigned int num_markers) {
  std::set<T> values;

  const T min = std::min(range.first, range.second);
  const T max = std::max(range.first, range.second);
  const T interval = (max - min) / (num_markers - 1);

  if (interval == 0) {
    return {min, max};
  }

  for (T marker = min; marker <= max; marker += interval) {
    values.insert(marker);
  }

  return values;
}

/**
 * \brief Partitions a range into a number of intermediate values
 * showing relevant values.
 */
template <typename T>
std::set<T> PartitionRange(const Interval<T>& range, unsigned int num_markers) {
  // TODO: Implement algorithm
  return TrivialPartitionRange(range, num_markers);
}

}  // namespace ranges

}  // namespace plotcpp

#endif  // _PLOTCPP_INCLUDE_UTILITY_HPP_
