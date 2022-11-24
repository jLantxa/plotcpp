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

#ifndef _PLOTCPP_APP_INCLUDE_APP_HPP_
#define _PLOTCPP_APP_INCLUDE_APP_HPP_

#include <string>
#include <vector>

#include "utility.hpp"

using DataSeries = std::vector<plotcpp::Real>;

struct DataCollection final {
  std::vector<std::string> labels;
  std::vector<DataSeries> series;
};

const std::string DEFAULT_CSV_DELIMITER{","};

#endif  // _PLOTCPP_APP_INCLUDE_APP_HPP_
