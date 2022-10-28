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

#include <gmock/gmock-matchers.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ranges.hpp"

namespace plotcpp {

using ::testing::ElementsAreArray;

TEST(RangesTest, TrivialPartitionRange) {
  EXPECT_THAT(ranges::TrivialPartitionRange({0, 10}, 5),
              ElementsAreArray({0.0f, 2.5f, 5.0f, 7.5f, 10.0f}));
  EXPECT_THAT(ranges::TrivialPartitionRange({-10, 10}, 5),
              ElementsAreArray({-10.0f, -5.0f, 0.0f, 5.0f, 10.0f}));
}

TEST(RangesTest, RangeGenerator) {
  EXPECT_THAT(ranges::Range(0, 10, 1),
              ElementsAreArray({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
  EXPECT_THAT(ranges::Range(10, 0, -1),
              ElementsAreArray({10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}));
  EXPECT_THAT(ranges::Range(-1.0f, 2.7f, 0.5f),
              ElementsAreArray(
                  {-1.0f, -0.5f, 0.0f, 0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 2.7f}));
  EXPECT_THAT(ranges::Range(3.0f, 0.1f, -0.5f),
              ElementsAreArray({3.0f, 2.5f, 2.0f, 1.5f, 1.0f, 0.5f, 0.1f}));
  EXPECT_THAT(ranges::Range(-5, 5, -0.1),
              ElementsAreArray(std::vector<Real>{}));
  EXPECT_THAT(ranges::Range(5, -5, 0.1), ElementsAreArray(std::vector<Real>{}));
}

TEST(RangesTest, FunctionGenerator) {
  const auto x = ranges::Range(0, 10, 1);
  const auto f = [](Real x) { return 2 * x; };
  const auto y_expected = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
  const auto y_generated = ranges::Generate(x, f);
  EXPECT_THAT(y_generated, ElementsAreArray(y_expected));
}

}  // namespace plotcpp
