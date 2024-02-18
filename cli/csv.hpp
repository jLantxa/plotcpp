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

#ifndef _PLOTCPP_APP_INCLUDE_CSV_HPP_
#define _PLOTCPP_APP_INCLUDE_CSV_HPP_

#include <string_view>

#include "cli.hpp"

DataCollection ParseCsv(const std::string &csv_filename,
                        std::string_view delimiter, bool first_line_has_labels);

#endif // _PLOTCPP_APP_INCLUDE_CSV_HPP_
