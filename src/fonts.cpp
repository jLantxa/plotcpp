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

#include "fonts.hpp"

#include <map>
#include <string>
#include <utility>

namespace plotcpp {
namespace fonts {

static const float DEFAULT_WIDTH_EM = 0.48f;

static const std::map<std::string, FontData> FONT_DATA = {
    {"monospace", {.width_em = 0.46f}},
};

float EmToPx(float em) { return em * 16.0f; }

std::pair<float, float> CalculateTextSize(const std::string& text,
                                          const std::string& font, float size) {
  const auto it = FONT_DATA.find(font);
  const float width_em =
      (it != FONT_DATA.end()) ? it->second.width_em : DEFAULT_WIDTH_EM;
  const std::size_t text_length = text.size();
  return {(static_cast<float>(text_length) * width_em) * (size / 12.0f),
          (size / 12.0f)};
}

}  // namespace fonts
}  // namespace plotcpp
