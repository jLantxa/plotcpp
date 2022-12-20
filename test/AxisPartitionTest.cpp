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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "utility.hpp"

TEST(AxisPartitionTest, TrivialPartitionRange) {
  EXPECT_THAT(plotcpp::ranges::TrivialPartitionRange<plotcpp::Real>({0, 10}, 5),
              ::testing::ElementsAreArray({0.0f, 2.5f, 5.0f, 7.5f, 10.0f}));
  EXPECT_THAT(
      plotcpp::ranges::TrivialPartitionRange<plotcpp::Real>({-10, 10}, 5),
      ::testing::ElementsAreArray({-10.0f, -5.0f, 0.0f, 5.0f, 10.0f}));
}
