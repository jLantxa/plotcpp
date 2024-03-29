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
#include <iterator>
#include <list>
#include <set>
#include <vector>

namespace plotcpp {

/** Internal real number type */
using Real = double;

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;

  constexpr Color() = default;

  constexpr Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}

  constexpr Color(uint32_t value) {
    b = value & 0xff;
    g = (value >> 8) & 0xFF;
    r = (value >> 16) & 0xFF;
  }
};

namespace adaptor {
/**
 * @brief Convert any numeric type to the internal real representation
 *
 * @tparam T Custom numeric type
 * @param x Value
 * @return Real Conversion from T to Real
 */
template <typename T> Real Real(T x) { return static_cast<::plotcpp::Real>(x); }

/**
 * @brief Convert a vector of any numeric type to the internal real
 * representation
 *
 * @tparam T Custom numeric type
 * @param v Vector
 * @return std::vector<Real> Conversion from T to Real
 */
template <typename T> std::vector<::plotcpp::Real> Real(std::vector<T> v) {
  const std::size_t size = v.size();

  std::vector<::plotcpp::Real> real_vector;
  real_vector.resize(size);

  for (std::size_t i = 0; i < size; ++i) {
    real_vector[i] = Real(v[i]);
  }

  return real_vector;
}

} // namespace adaptor

namespace ranges {

template <typename T> using Interval = std::pair<T, T>;

/**
 * @brief Returns a vector of Real numbers from a to b in equally spaced
 * intervals except, maybe, the last interval.
 *
 * @tparam T Custom numeric type
 */
template <typename T> std::vector<T> MakeRange(T start, T end, T step) {
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
 * @brief Generates a vector of Real y as a function of a vector of Real x.
 *
 * @param x Input vector
 * @param function A function f such that y=f(x)
 * @return A vector y such that y=function(x)
 */
template <typename T>
std::vector<T> Generate(const std::vector<T> &x,
                        const std::function<T(T)> &function) {
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
 * @brief Partitions a range into a number of intermediate values
 * from the minumum value to the maximum value.
 */
template <typename T>
std::set<T> TrivialPartitionRange(const Interval<T> &range,
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
 * @brief Partitions a range into a number of intermediate values
 * showing relevant values.
 */
template <typename T>
std::set<T> PartitionRange(const Interval<T> &range, unsigned int num_markers) {
  // TODO: Implement algorithm
  return TrivialPartitionRange(range, num_markers);
}

} // namespace ranges

template <typename T>
std::pair<std::size_t, bool>
BinarySearchInterval(T value, const std::vector<T> &intervals) {
  const std::size_t num_intervals = intervals.size();

  if (num_intervals == 0) {
    return {0, false};
  } else if ((num_intervals == 1) && (value == intervals.front())) {
    return {0, true};
  }

  if ((value < intervals.front()) || (value > intervals.back())) {
    return {0, false};
  }

  std::size_t low = 0;
  std::size_t high = num_intervals - 1;
  std::size_t index = 0;
  while (low != high) {
    index = (low + high) / 2;

    if ((value >= intervals[index]) && (value <= intervals[index + 1])) {
      break;
    }

    if (value > intervals[index + 1]) {
      low = index;
    } else {
      high = index;
    }
  }

  return {index, true};
}

namespace color_tables {

const std::vector<Color> BRIGHT{
    Color(0x4477AA), Color(0xEE6677), Color(0x228833), Color(0xCCBB44),
    Color(0x66CCEE), Color(0xAA3377), Color(0xBBBBBB)};

const std::vector<Color> VIBRANT{
    Color(0xEE7733), Color(0x0077BB), Color(0x33BBEE), Color(0xEE3377),
    Color(0xCC3311), Color(0x009988), Color(0xBBBBBB)};

const std::vector<Color> MUTED{
    Color(0xCC6677), Color(0x332288), Color(0xDDCC77), Color(0x117733),
    Color(0x88CCEE), Color(0x882255), Color(0x44AA99), Color(0x999933)};

const std::vector<Color> LIGHT{
    Color(0x77AADD), Color(0xEE8866), Color(0xEEDD88),
    Color(0xFFAABB), Color(0x99DDFF), Color(0x44BB99),
    Color(0xBBCC33), Color(0xAAAA00), Color(0xDDDDDD)};

} // namespace color_tables

class ColorSelector final {
public:
  ColorSelector(const std::vector<Color> &table) : m_table(table) {}
  const Color &NextColor() {
    const Color &color = m_table[index];
    index = (index + 1) % m_table.size();
    return color;
  }

private:
  const std::vector<Color> m_table;

  std::size_t index = 0;
};

} // namespace plotcpp

#endif // _PLOTCPP_INCLUDE_UTILITY_HPP_
