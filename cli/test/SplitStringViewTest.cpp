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

#include <string>
#include <string_view>

#include "SplitStringView.hpp"

using ::testing::ElementsAreArray;

struct TestValues {
  std::string text;
  std::string delimiter;
  std::vector<std::string> expected_values;
};

class SplitStringViewTestSuite : public ::testing::TestWithParam<TestValues> {};

TEST_P(SplitStringViewTestSuite, SplitStringViewTestP) {
  TestValues values = GetParam();

  std::vector<std::string> found_tokens;
  for (const auto word : SplitStringView(values.text, values.delimiter)) {
    found_tokens.push_back(std::string{word});
  }

  EXPECT_THAT(found_tokens, ElementsAreArray(values.expected_values));
}

INSTANTIATE_TEST_SUITE_P(
    SingleCharDelimiter, SplitStringViewTestSuite,
    ::testing::Values(TestValues{
        "split,string,view,text", ",", {"split", "string", "view", "text"}}));

INSTANTIATE_TEST_SUITE_P(
    MultiCharDelimiter, SplitStringViewTestSuite,
    ::testing::Values(TestValues{
        "I<=>love<=>coffee", "<=>", {"I", "love", "coffee"}}));

INSTANTIATE_TEST_SUITE_P(NoDelimiter, SplitStringViewTestSuite,
                         ::testing::Values(TestValues{
                             "hello", ";", {"hello"}}));

INSTANTIATE_TEST_SUITE_P(TrailingDelimiter, SplitStringViewTestSuite,
                         ::testing::Values(TestValues{
                             "hello,", ",", {"hello"}}));

INSTANTIATE_TEST_SUITE_P(SpaceAfterDelimiter, SplitStringViewTestSuite,
                         ::testing::Values(TestValues{
                             "hello, ", ",", {"hello", " "}}));

INSTANTIATE_TEST_SUITE_P(WrongDelimiter, SplitStringViewTestSuite,
                         ::testing::Values(TestValues{
                             "i,made,a,mistake", ";", {"i,made,a,mistake"}}));
