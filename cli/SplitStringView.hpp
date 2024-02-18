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

#ifndef _PLOTCPP_APP_INCLUDE_SPLIT_STRING_HPP_
#define _PLOTCPP_APP_INCLUDE_SPLIT_STRING_HPP_

#include <string>
#include <string_view>

class SplitStringView final {
public:
  SplitStringView(const std::string_view str, std::string_view delimiter);

  class Iterator final {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::string_view;
    using pointer = std::string_view;
    using reference = std::string_view;

    Iterator(const SplitStringView &split_str_view, std::size_t start);

    void FindNextToken();

    bool operator==(const Iterator &other) const;

    bool operator!=(const Iterator &other) const;

    Iterator &operator++();

    reference operator*();

    pointer operator->();

  private:
    const SplitStringView &split_string_view;
    std::size_t substr_start;
    std::size_t substr_end;
    std::size_t next_token = 0;
  };

  Iterator begin();
  Iterator end();

private:
  const std::string_view string;
  const std::string_view delimiter;

  friend class ::SplitStringView::Iterator;
};

#endif // _PLOTCPP_APP_INCLUDE_SPLIT_STRING_HPP_
