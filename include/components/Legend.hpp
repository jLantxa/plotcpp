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

#ifndef _PLOTCPP_INCLUDE_COMPONENTS_LEGEND_HPP_
#define _PLOTCPP_INCLUDE_COMPONENTS_LEGEND_HPP_

#include "components/text.hpp"
#include "fonts.hpp"
#include "style.hpp"
#include "svg.hpp"
#include "utility.hpp"

namespace plotcpp {
namespace components {

class Legend {
 public:
  enum class Alignment {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
  };

  enum class DataType {
    LINE,
    POINT,
    BAR,
  };

  struct Style {
    DataType type;
    Color color;
    std::string dash_array{""};
  };

  Legend() = default;

  void AddEntry(const std::string& label, const Style& style);

  void Draw(svg::Document* document, float x, float y, float margin,
            Alignment alignment) const;

 protected:
  using LegendEntry = std::pair<std::string, Style>;
  std::vector<LegendEntry> m_legend_labels;

  static constexpr float FONT_SIZE = 12.0f;
  static constexpr float FONT_EM = FONT_SIZE / 12.0f;
  static constexpr float FONT_MARGIN_EM = 0.5f * FONT_EM;
  static constexpr float SYMBOL_LENTH_EM = 1.5f * FONT_EM;
  static constexpr float RECT_LENGTH_EM = 3.0f * FONT_EM / 4.0f;
  static constexpr float RECT_LENGTH_PX = fonts::EmToPx(RECT_LENGTH_EM);
  static constexpr float SPACING_LENGTH_EM = 0.5f * FONT_EM;
  static constexpr Color STROKE_COLOR = style::BORDER_COLOR;
};

}  // namespace components
}  // namespace plotcpp

#endif  // _PLOTCPP_INCLUDE_COMPONENTS_LEGEND_HPP_
