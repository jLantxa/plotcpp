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

#include "components/Legend.hpp"

#include "fonts.hpp"
#include "svg.hpp"

namespace plotcpp {
namespace components {

void Legend::AddEntry(const std::string& label, const Style& style) {
  m_legend_labels.emplace_back(LegendEntry{label, style});
}

void Legend::Draw(svg::Document* document, float x, float y, float margin,
                  Alignment alignment) const {
  if (m_legend_labels.empty()) {
    return;
  }

  // Calculate sizes
  float max_font_width_em = 0;
  for (const auto& label : m_legend_labels) {
    const auto [w, _] =
        fonts::CalculateTextSize(label.first, components::TEXT_FONT, FONT_SIZE);
    max_font_width_em = std::max(max_font_width_em, w);
  }

  const std::size_t num_labels = m_legend_labels.size();
  const float box_w = fonts::EmToPx(2 * FONT_MARGIN_EM + SYMBOL_LENTH_EM +
                                    SPACING_LENGTH_EM + max_font_width_em);
  const float box_h = fonts::EmToPx(static_cast<float>(num_labels) * FONT_EM +
                                    2 * FONT_MARGIN_EM);

  // Apply alignment
  switch (alignment) {
    case Alignment::TOP_LEFT:
      x += margin;
      y += margin;
      break;
    case Alignment::TOP_RIGHT:
      x -= (box_w + margin);
      y += margin;
      break;
    case Alignment::BOTTOM_LEFT:
      x += margin;
      y -= (box_h + margin);
      break;
    case Alignment::BOTTOM_RIGHT:
      x -= (box_w + margin);
      y -= (box_h - margin);
      break;
  }

  svg::Rect box_rect = {.x = x,
                        .y = y,
                        .width = box_w,
                        .height = box_h,
                        .stroke_color = STROKE_COLOR};
  auto box_rect_node = document->DrawRect(box_rect);
  svg::SetAttribute(box_rect_node, "fill", "white");
  svg::SetAttribute(box_rect_node, "fill-opacity", "0.90");
  svg::SetAttribute(box_rect_node, "rx", "4", "px");
  svg::SetAttribute(box_rect_node, "ry", "4", "px");

  for (std::size_t i = 0; i < num_labels; ++i) {
    const LegendEntry label = m_legend_labels[i];
    const float element_y = y + fonts::EmToPx(FONT_EM / 2 + FONT_MARGIN_EM +
                                              static_cast<float>(i) * FONT_EM);

    if (label.second.type == DataType::LINE) {
      svg::Line dash = {.x1 = x + fonts::EmToPx(FONT_MARGIN_EM),
                        .y1 = element_y,
                        .x2 = x + fonts::EmToPx(FONT_MARGIN_EM) +
                              fonts::EmToPx(SYMBOL_LENTH_EM),
                        .y2 = element_y,
                        .stroke_color = label.second.color,
                        .stroke_width = 2.0f};
      auto line_node = document->DrawLine(dash);
      svg::SetAttribute(line_node, "stroke-dasharray", label.second.dash_array);
    } else if (label.second.type == DataType::POINT) {
      svg::Circle circle{
          .cx = x + fonts::EmToPx(FONT_MARGIN_EM) +
                (fonts::EmToPx(SYMBOL_LENTH_EM) / 2.0f),
          .cy = element_y,
          .r = fonts::EmToPx(FONT_EM / 3.0f),
          .fill_color = label.second.color,
      };
      document->DrawCircle(circle);
    } else if (label.second.type == DataType::BAR) {
      svg::Rect color_rect{
          .x = x + fonts::EmToPx(FONT_MARGIN_EM + (SYMBOL_LENTH_EM / 2.0f)) -
               (RECT_LENGTH_PX / 2.0f),
          .y = element_y - (RECT_LENGTH_PX / 2.0f),
          .width = RECT_LENGTH_PX,
          .height = RECT_LENGTH_PX,
          .stroke_color = m_legend_labels[i].second.color,
          .stroke_opacity = 1.0f,
          .stroke_width = 1.0f,
          .fill_color = m_legend_labels[i].second.color,
          .fill_opacity = 1.0f,
          .fill_transparent = false};
      document->DrawRect(color_rect);
    }

    const float text_x =
        x + fonts::EmToPx(FONT_MARGIN_EM + SYMBOL_LENTH_EM + SPACING_LENGTH_EM);
    const float text_y = element_y + fonts::EmToPx(FONT_EM / 4);
    document->DrawText(svg::Text{label.first, text_x, text_y, FONT_SIZE,
                                 components::TEXT_FONT});
  }
}

}  // namespace components
}  // namespace plotcpp
