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

#include "csv.hpp"

#include <fstream>
#include <string>
#include <string_view>

#include "SplitStringView.hpp"

DataCollection ParseCsv(const std::string &csv_filename,
                        std::string_view delimiter,
                        bool first_line_has_labels) {
  DataCollection collection;

  std::fstream file_stream(csv_filename);
  std::string line;

  if (first_line_has_labels) {
    if (std::getline(file_stream, line) && (line != "")) {
      SplitStringView split_view(line, delimiter);
      for (const auto &label : split_view) {
        collection.labels.emplace_back(label);
      }
    }
  }

  while (std::getline(file_stream, line)) {
    if (line == "") {
      continue;
    }

    DataSeries new_series;
    SplitStringView split_view(line, delimiter);
    for (const auto &token : split_view) {
      new_series.emplace_back(static_cast<plotcpp::Real>(atof(token.data())));
    }
    collection.series.emplace_back(new_series);
  }

  return collection;
}
