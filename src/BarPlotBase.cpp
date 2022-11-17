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

#include "BarPlotBase.hpp"

#include <fmt/format.h>

#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "fonts.hpp"
#include "svg.hpp"
#include "utility.hpp"

namespace plotcpp {

const std::string BarPlotBase::FRAME_RECT_CLIP_PATH_ID = {"rect-clip-path"};

void BarPlotBase::SetXLabel(const std::string& label) { m_x_label = label; }

void BarPlotBase::SetYLabel(const std::string& label) { m_y_label = label; }

void BarPlotBase::SetGridEnable(bool enable) { m_grid_enable = enable; }

void BarPlotBase::SetRoundedEdges(bool enable) { m_rounded_borders = enable; }

void BarPlotBase::SetBarRelativeWidth(float rel_width) {
  m_bar_width_rel = std::max(0.0f, std::min(1.0f, rel_width));
}

void BarPlotBase::AddYMarker(Real marker) { m_y_custom_markers.insert(marker); }

void BarPlotBase::Clear() {
  ClearData();

  m_x_label.clear();
  m_y_label.clear();
}

void BarPlotBase::ClearData() {
  m_baselines.clear();
  m_numeric_x_data.clear();
  m_categorical_x_data.clear();
  m_y_data.clear();
}

void BarPlotBase::Build() {
  m_svg.Reset();
  m_svg.SetSize(m_width, m_height);

  CalculateFrame();

  DrawBackground();
  DrawTitle();
  DrawXLabel();
  DrawYLabel();
  DrawXAxis();
  DrawYAxis();
  DrawBars();
  DrawFrame();
}

float BarPlotBase::TranslateToFrame(Real y) const {
  return m_bar_top_y - (m_zoom_y * static_cast<float>(y - m_y_range.second));
}

void BarPlotBase::CalculateFrame() {
  // Set default baselines if not set
  if (m_baselines.size() == 0) {
    m_baselines = std::vector<Real>(m_num_bars, 0.0f);
  }

  // Frame rectangle
  m_frame_x = static_cast<float>(m_width) * FRAME_LEFT_MARGIN_REL;
  m_frame_y = static_cast<float>(m_height) * (FRAME_TOP_MARGIN_REL);
  m_frame_w = static_cast<float>(m_width) *
              (1.0f - FRAME_LEFT_MARGIN_REL - FRAME_RIGHT_MARGIN_REL);
  m_frame_h = static_cast<float>(m_height) *
              (1.0f - FRAME_TOP_MARGIN_REL - FRAME_BOTTOM_MARGIN_REL);
  m_bar_top_y = m_frame_y + (BAR_FRAME_Y_MARGIN_REL * m_frame_h);

  // Calculate y range
  std::vector<Real> pos_acc(m_num_bars, 0.0f);
  std::vector<Real> neg_acc(m_num_bars, 0.0f);
  for (const auto& series : m_y_data) {
    for (std::size_t i = 0; i < m_num_bars; ++i) {
      const Real value = series.values[i];
      if (value >= 0.0f) {
        pos_acc[i] += series.values[i];
      } else {
        neg_acc[i] += series.values[i];
      }
    }
  }
  for (std::size_t i = 0; i < m_num_bars; ++i) {
    pos_acc[i] += m_baselines[i];
    neg_acc[i] += m_baselines[i];
  }

  m_y_range =
      std::pair<Real, Real>{*std::min_element(neg_acc.begin(), neg_acc.end()),
                            *std::max_element(pos_acc.begin(), pos_acc.end())};

  m_zoom_y = std::abs((m_frame_h * (1 - 2 * BAR_FRAME_Y_MARGIN_REL)) /
                      static_cast<float>(m_y_range.second - m_y_range.first));

  const unsigned int num_y_markers =
      std::min(MAX_NUM_Y_MARKERS,
               static_cast<unsigned int>(m_frame_h / PIXELS_PER_Y_MARKER));
  m_y_markers = ranges::PartitionRange(m_y_range, num_y_markers);

  // Calculate font sizes
  const std::string DUMMY_TEXT = "-000.00";
  const float y_axis_font_size =
      fonts::ConstrainedFontSize(BASE_AXIS_FONT_SIZE, DUMMY_TEXT, TEXT_FONT,
                                 (3.0f * m_frame_x / 4.0f) - MARKER_LENGTH,
                                 m_frame_h / static_cast<float>(num_y_markers));
  m_axis_font_size = std::min({y_axis_font_size});
}

void BarPlotBase::DrawBars() {
  // Horizontal space for a bar including a relative margin
  const float bar_horizontal_space =
      (m_frame_w * (1 - 2 * BAR_FRAME_Y_MARGIN_REL)) /
      static_cast<float>(m_num_bars);
  const float bar_width = bar_horizontal_space * m_bar_width_rel;

  std::vector<std::size_t> remaining_positive_segment_counts =
      std::vector<std::size_t>(m_num_bars, 0);
  std::vector<std::size_t> remaining_negative_segment_counts =
      std::vector<std::size_t>(m_num_bars, 0);
  std::vector<Real> positive_bar_sizes(m_num_bars, 0.0f);
  std::vector<Real> negative_bar_sizes(m_num_bars, 0.0f);
  for (const auto& data_series : m_y_data) {
    for (std::size_t i = 0; i < m_num_bars; ++i) {
      const Real value = data_series.values[i];
      if (value > 0) {
        ++remaining_positive_segment_counts[i];
        positive_bar_sizes[i] += value;
      } else if (value < 0) {
        ++remaining_negative_segment_counts[i];
        negative_bar_sizes[i] += value;
      }
    }
  }

  std::vector<Real> pos_acc(m_num_bars, 0.0f);
  std::vector<Real> neg_acc(m_num_bars, 0.0f);
  for (const auto& series : m_y_data) {
    for (std::size_t i = 0; i < m_num_bars; ++i) {
      const Real value = series.values[i];

      float start_y;
      float end_y;
      bool should_round_border = m_rounded_borders;
      Real bar_height = 0.0f;
      if (value >= 0) {
        start_y = TranslateToFrame(m_baselines[i] + pos_acc[i]);
        end_y = TranslateToFrame(m_baselines[i] + pos_acc[i] + value);
        pos_acc[i] += value;
        bar_height = TranslateToFrame(positive_bar_sizes[i]) -
                     TranslateToFrame(m_baselines[i]);
        should_round_border &= (--remaining_positive_segment_counts[i] == 0);
      } else {
        start_y = TranslateToFrame(m_baselines[i] + neg_acc[i]);
        end_y = TranslateToFrame(m_baselines[i] + neg_acc[i] + value);
        neg_acc[i] += value;
        bar_height = TranslateToFrame(negative_bar_sizes[i]) -
                     TranslateToFrame(m_baselines[i]);
        should_round_border &= (--remaining_negative_segment_counts[i] == 0);
      }

      const float bar_center_x = m_frame_x +
                                 (m_frame_w * BAR_FRAME_X_MARGIN_REL) +
                                 (bar_horizontal_space / 2.0f) +
                                 (static_cast<float>(i) * bar_horizontal_space);

      static constexpr float max_radius = 5.0f;
      const float radius = std::min({max_radius, bar_width / 2.0f,
                                     static_cast<float>(std::abs(bar_height))});
      const float delta = (value >= 0) ? -radius : radius;

      std::vector<svg::PathCommand> cmds =
          (!should_round_border)
              ? std::vector<
                    svg::PathCommand>{{svg::PathCommand::Id::MOVE,
                                       {bar_center_x - (bar_width / 2.0f),
                                        start_y}},
                                      {svg::PathCommand::Id::VERTICAL, {end_y}},
                                      {svg::PathCommand::Id::HORIZONTAL_R,
                                       {bar_width}},
                                      {svg::PathCommand::Id::VERTICAL,
                                       {start_y}},
                                      {svg::PathCommand::Id::CLOSE, {}}}
              : std::vector<svg::PathCommand>{
                    {svg::PathCommand::Id::MOVE,
                     {bar_center_x - (bar_width / 2.0f), start_y}},
                    {svg::PathCommand::Id::VERTICAL, {end_y - delta}},
                    {svg::PathCommand::Id::QUADRATIC_R,
                     {0, delta, std::abs(delta), delta}},
                    {svg::PathCommand::Id::HORIZONTAL,
                     {bar_center_x + bar_width / 2.0f - std::abs(delta)}},
                    {svg::PathCommand::Id::QUADRATIC_R,
                     {std::abs(delta), 0, std::abs(delta), -delta}},
                    {svg::PathCommand::Id::VERTICAL, {start_y}},
                    {svg::PathCommand::Id::CLOSE, {}}};

      svg::Path path{
          .commands = cmds,
          .stroke_width = 1,
          .stroke_color = series.color,
          .stroke_opacity = 1.0f,
          .fill_color = series.color,
          .fill_opacity = 1.0f,
          .fill_transparent = false,
      };
      m_svg.DrawPath(path);
    }
  }
}

void BarPlotBase::DrawBackground() { m_svg.DrawBackground(BACKGROUND_COLOR); }

void BarPlotBase::DrawXAxis() {
  if (m_num_bars <= 1) {
    // TODO: Draw marker in the middle
    return;
  }

  const std::size_t max_num_x_markers =
      static_cast<std::size_t>(m_frame_w / PIXELS_PER_X_MARKER);
  const std::size_t marker_step = m_num_bars / max_num_x_markers;

  const float bar_horizontal_space =
      (m_frame_w * (1 - 2 * BAR_FRAME_Y_MARGIN_REL)) /
      static_cast<float>(m_num_bars);

  for (std::size_t i = 0; i < m_num_bars; i += marker_step) {
    const float x = m_frame_x + (m_frame_w * BAR_FRAME_X_MARGIN_REL) +
                    (bar_horizontal_space / 2.0f) +
                    (static_cast<float>(i) * bar_horizontal_space);
    const auto y = m_frame_y + m_frame_h;

    svg::Line marker_line{.x1 = x,
                          .y1 = y,
                          .x2 = x,
                          .y2 = y + MARKER_LENGTH,
                          .stroke_color = FRAME_STROKE_COLOR,
                          .stroke_opacity = 1.0f,
                          .stroke_width = 1};
    m_svg.DrawLine(marker_line);

    const std::string marker_text =
        (m_data_type == DataType::NUMERIC)
            ? fmt::format("{:.2f}", m_numeric_x_data[i])
            : m_categorical_x_data[i];
    float font_em = m_axis_font_size / 12.0f;
    auto text_node = m_svg.DrawText(
        svg::Text{marker_text, x, y + MARKER_LENGTH + fonts::EmToPx(font_em),
                  m_axis_font_size, TEXT_FONT});
    svg::SetAttribute(text_node, "text-anchor", "middle");

    if (m_grid_enable) {
      svg::Line grid_line{.x1 = x,
                          .y1 = m_frame_y,
                          .x2 = x,
                          .y2 = m_frame_y + m_frame_h,
                          .stroke_color = FRAME_STROKE_COLOR,
                          .stroke_opacity = 1.0f,
                          .stroke_width = 0.75f};
      auto grid_line_node = m_svg.DrawLine(grid_line);
      svg::SetAttribute(grid_line_node, "stroke-dasharray", "0.75,0.75");
    }
  }
}

void BarPlotBase::DrawYAxis() {
  std::set<Real> markers;
  markers.insert(m_y_markers.begin(), m_y_markers.end());
  markers.insert(m_y_custom_markers.begin(), m_y_custom_markers.end());

  if (markers.size() <= 1) {
    // TODO: Draw marker in the middle
    return;
  }

  for (const auto& marker : markers) {
    if ((marker < m_y_range.first) || (marker > m_y_range.second)) {
      continue;
    }

    const auto y = TranslateToFrame(marker);
    const float x = m_frame_x;

    svg::Line marker_line{.x1 = x,
                          .y1 = y,
                          .x2 = x - MARKER_LENGTH,
                          .y2 = y,
                          .stroke_color = FRAME_STROKE_COLOR,
                          .stroke_opacity = 1.0f,
                          .stroke_width = 1};
    m_svg.DrawLine(marker_line);

    float font_em =
        m_axis_font_size /
        12.0f;  // Some SVG renderers don't support baseline alignment
    const std::string marker_text =
        (!m_round_y_markers)
            ? fmt::format("{:.2f}", marker)
            : fmt::format("{:d}", static_cast<int>(std::round(marker)));

    auto text_node = m_svg.DrawText(svg::Text{
        marker_text, x - 2 * MARKER_LENGTH, y + fonts::EmToPx(font_em / 4.0f),
        m_axis_font_size, TEXT_FONT});
    svg::SetAttribute(text_node, "text-anchor", "end");

    if (m_grid_enable) {
      svg::Line grid_line{.x1 = m_frame_x,
                          .y1 = y,
                          .x2 = m_frame_x + m_frame_w,
                          .y2 = y,
                          .stroke_color = FRAME_STROKE_COLOR,
                          .stroke_opacity = 1.0f,
                          .stroke_width = 0.75f};
      auto grid_line_node = m_svg.DrawLine(grid_line);
      svg::SetAttribute(grid_line_node, "stroke-dasharray", "0.75,0.75");
    }
  }
}

void BarPlotBase::DrawXLabel() {
  if (m_x_label.empty()) {
    return;
  }

  const float frame_bottom = m_frame_y + m_frame_h;
  const float x = m_frame_x + (m_frame_w / 2);
  const float y =
      frame_bottom + (0.75f) * (static_cast<float>(m_height) - frame_bottom);

  auto node_ptr =
      m_svg.DrawText(svg::Text{m_x_label, x, y, m_axis_font_size, TEXT_FONT});
  svg::SetAttribute(node_ptr, "text-anchor", "middle");
}

void BarPlotBase::DrawYLabel() {
  if (m_x_label.empty()) {
    return;
  }

  const float x = (1 - 0.75f) * m_frame_x;
  const float y = m_frame_y + (m_frame_h / 2);

  auto node_ptr =
      m_svg.DrawText(svg::Text{m_y_label, 0, 0, m_axis_font_size, TEXT_FONT});
  svg::SetAttribute(node_ptr, "text-anchor", "middle");

  std::stringstream trans_ss;
  trans_ss << "translate(" << std::to_string(x) << ", " << std::to_string(y)
           << ") "
           << "rotate(-90)";
  svg::SetAttribute(node_ptr, "transform", trans_ss.str());
}

void BarPlotBase::DrawFrame() {
  svg::Rect frame_rect{
      .x = m_frame_x,
      .y = m_frame_y,
      .width = m_frame_w,
      .height = m_frame_h,
      .stroke_color = FRAME_STROKE_COLOR,
  };
  auto frame_node = m_svg.DrawRect(frame_rect);
  svg::SetAttribute(frame_node, "fill", "none");

  auto defs_node = m_svg.Defs();
  auto clip_path_node = svg::AppendNode(defs_node, "clipPath");
  svg::SetAttribute(clip_path_node, "id", FRAME_RECT_CLIP_PATH_ID);
  svg::Rect clip_rect{
      .x = m_frame_x, .y = m_frame_y, .width = m_frame_w, .height = m_frame_h};
  m_svg.DrawRect(clip_rect, clip_path_node);
}

void BarPlotBase::DrawTitle() {
  if (m_title.empty()) {
    return;
  }

  const float x = static_cast<float>(m_width) / 2;
  const float y = static_cast<float>(m_height) * FRAME_TOP_MARGIN_REL / 2;

  const float font_size = fonts::ConstrainedFontSize(
      BASE_TITLE_FONT_SIZE, m_title, TEXT_FONT, static_cast<float>(m_width),
      static_cast<float>(m_height) * FRAME_TOP_MARGIN_REL);

  auto node_ptr =
      m_svg.DrawText(svg::Text{m_title, x, y, font_size, TEXT_FONT});
  svg::SetAttribute(node_ptr, "font-weight", "bold");
  svg::SetAttribute(node_ptr, "text-anchor", "middle");
}

}  // namespace plotcpp
