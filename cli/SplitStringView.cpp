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

#include "SplitStringView.hpp"

#include <string_view>

SplitStringView::SplitStringView(const std::string_view str,
                                 std::string_view delimiter)
    : string(str), delimiter(delimiter) {}

SplitStringView::Iterator SplitStringView::begin() {
  return Iterator(*this, 0);
}

SplitStringView::Iterator SplitStringView::end() {
  return Iterator(*this, std::string_view::npos);
}

SplitStringView::Iterator::Iterator(const SplitStringView &split_str_view,
                                    std::size_t start)
    : split_string_view(split_str_view), next_token(start) {
  FindNextToken();
}

void SplitStringView::Iterator::FindNextToken() {
  if (next_token >= split_string_view.string.size()) {
    substr_start = std::string_view::npos;
    substr_end = std::string_view::npos;
    next_token = std::string_view::npos;
    return;
  }

  substr_start = next_token;
  const std::string_view substr_view =
      split_string_view.string.substr(substr_start);
  const std::size_t delimiter_index =
      substr_view.find(split_string_view.delimiter);

  if (delimiter_index != std::string_view::npos) {
    substr_end = substr_start + delimiter_index;
    next_token = substr_end + split_string_view.delimiter.size();
  } else {
    substr_end = split_string_view.string.size();
    next_token = substr_end;
  }
}

bool SplitStringView::Iterator::operator==(const Iterator &other) const {
  return (split_string_view.string == other.split_string_view.string) &&
         (split_string_view.delimiter == other.split_string_view.delimiter) &&
         (substr_start == other.substr_start) &&
         (substr_end == other.substr_end) && (next_token == other.next_token);
}

bool SplitStringView::Iterator::operator!=(const Iterator &other) const {
  return !(*this == other);
}

SplitStringView::Iterator &SplitStringView::Iterator::operator++() {
  FindNextToken();
  return *this;
}

SplitStringView::Iterator::reference SplitStringView::Iterator::operator*() {
  return split_string_view.string.substr(substr_start,
                                         substr_end - substr_start);
}

SplitStringView::Iterator::pointer SplitStringView::Iterator::operator->() {
  return split_string_view.string.substr(substr_start,
                                         substr_end - substr_start);
}
