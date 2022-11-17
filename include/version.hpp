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

#ifndef _PLOTCPP_INCLUDE_VERSION_HPP_
#define _PLOTCPP_INCLUDE_VERSION_HPP_

#include <cassert>
#include <cstring>

namespace plotcpp {

static const char* HEADER_VERSION = "v0.2.0";

const char* GetBuildVersion();

[[maybe_unused]] static bool TestVersion() {
  const char* build_version = GetBuildVersion();
  return (strcmp(build_version, HEADER_VERSION) == 0);
}

}  // namespace plotcpp

#endif  // _PLOTCPP_INCLUDE_VERSION_HPP_
