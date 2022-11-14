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

#include <utility>

#include "utility.hpp"

namespace plotcpp {

using ::testing::ElementsAreArray;

TEST(RangesTest, TrivialPartitionRange) {
  EXPECT_THAT(ranges::TrivialPartitionRange<Real>({0, 10}, 5),
              ElementsAreArray({0.0f, 2.5f, 5.0f, 7.5f, 10.0f}));
  EXPECT_THAT(ranges::TrivialPartitionRange<Real>({-10, 10}, 5),
              ElementsAreArray({-10.0f, -5.0f, 0.0f, 5.0f, 10.0f}));
}

TEST(UtilityTest, RangeGenerator) {
  EXPECT_THAT(ranges::MakeRange<int>(0, 10, 1),
              ElementsAreArray({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
  EXPECT_THAT(ranges::MakeRange(10, 0, -1),
              ElementsAreArray({10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}));
  EXPECT_THAT(ranges::MakeRange<float>(-1.0f, 2.7f, 0.5f),
              ElementsAreArray(
                  {-1.0f, -0.5f, 0.0f, 0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 2.7f}));
  EXPECT_THAT(ranges::MakeRange<double>(3.0f, 0.1f, -0.5f),
              ElementsAreArray({3.0f, 2.5f, 2.0f, 1.5f, 1.0f, 0.5f, 0.1f}));
  EXPECT_THAT(ranges::MakeRange<Real>(-5, 5, -0.1f),
              ElementsAreArray(std::vector<Real>{}));
  EXPECT_THAT(ranges::MakeRange<float>(5, -5, 0.1f),
              ElementsAreArray(std::vector<float>{}));
}

TEST(UtilityTest, FunctionGenerator) {
  const std::vector<int> x = ranges::MakeRange<int>(0, 10, 1);
  const auto f = [](int x) { return 2 * x; };
  const std::vector<int> y_expected = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
  const std::vector<int> y_generated = ranges::Generate<int>(x, f);
  EXPECT_THAT(y_generated, ElementsAreArray(y_expected));
}

TEST(UtilityTest, BinarySearchInterval) {
  using Pair = std::pair<std::size_t, bool>;
  std::vector<float> intervals{-2.5, -1.0, 0.0f, 1.5f, 3.5f, 5.2f};

  EXPECT_EQ(BinarySearchInterval(1.0f, intervals), (Pair{2, true}));
  EXPECT_EQ(BinarySearchInterval(-1.0f, intervals), (Pair{1, true}));
  EXPECT_EQ(BinarySearchInterval(4.0f, intervals), (Pair{4, true}));
  EXPECT_EQ(BinarySearchInterval(-3.0f, intervals), (Pair{0, false}));
  EXPECT_EQ(BinarySearchInterval(6.0f, intervals), (Pair{0, false}));
}

}  // namespace plotcpp
