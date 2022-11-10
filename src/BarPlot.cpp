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

#include "BarPlot.hpp"

#include <algorithm>

#include "fonts.hpp"
#include "svg.hpp"

namespace plotcpp {

const std::string BarPlotBase::FRAME_RECT_CLIP_PATH_ID = {"rect-clip-path"};

void BarPlotBase::SetXLabel(const std::string& label) { m_x_label = label; }

void BarPlotBase::SetYLabel(const std::string& label) { m_y_label = label; }

void BarPlotBase::SetGridEnable(bool enable) { m_grid_enable = enable; }

void BarPlotBase::SetRoundedEdges(bool enable) { m_rounded_borders = enable; }

void BarPlotBase::SetBarRelativeWidth(float rel_width) {
  m_bar_width_rel = std::max(0.0f, std::min(1.0f, rel_width));
}

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
  DrawBars();
  DrawFrame();
  DrawXLabel();
  DrawYLabel();
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
  for (const auto& data_series : m_y_data) {
    for (std::size_t i = 0; i < m_num_bars; ++i) {
      const Real value = data_series.values[i];
      if (value > 0) {
        remaining_positive_segment_counts[i]++;
      } else if (value < 0) {
        remaining_negative_segment_counts[i]++;
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
      bool should_round_border;
      if (value >= 0) {
        start_y = TranslateToFrame(m_baselines[i] + pos_acc[i]);
        end_y = TranslateToFrame(m_baselines[i] + pos_acc[i] + value);
        pos_acc[i] += value;
        should_round_border =
            m_rounded_borders && (--remaining_positive_segment_counts[i] == 0);
      } else {
        start_y = TranslateToFrame(m_baselines[i] + neg_acc[i]);
        end_y = TranslateToFrame(m_baselines[i] + neg_acc[i] + value);
        neg_acc[i] += value;
        should_round_border =
            m_rounded_borders && (--remaining_negative_segment_counts[i] == 0);
      }

      const float bar_center_x = m_frame_x +
                                 (m_frame_w * BAR_FRAME_X_MARGIN_REL) +
                                 (bar_horizontal_space / 2.0f) +
                                 (static_cast<float>(i) * bar_horizontal_space);

      static constexpr float max_radius = 5.0f;
      float radius = std::min(max_radius, bar_width / 2.0f);
      float delta = (value >= 0) ? -radius : radius;

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

void BarPlot::Plot(const std::vector<Real>& x_data,
                   const std::vector<Real>& y_data, const Color& color) {
  if (x_data.size() != y_data.size()) {
    return;
  }

  if (m_y_data.size() == 0) {
    m_num_bars = x_data.size();
  } else {
    if (x_data.size() != m_num_bars) {
      return;
    }
  }

  if (m_data_type != DataType::NUMERIC) {
    m_categorical_x_data.clear();
    m_y_data.clear();
  }

  m_data_type = DataType::NUMERIC;
  m_numeric_x_data = x_data;
  m_y_data.emplace_back(DataSeries{y_data, color});
}

void BarPlot::Plot(const std::vector<std::string>& x_data,
                   const std::vector<Real>& y_data, const Color& color) {
  if (x_data.size() != y_data.size()) {
    return;
  }

  if (m_y_data.size() == 0) {
    m_num_bars = x_data.size();
  } else {
    if (x_data.size() != m_num_bars) {
      return;
    }
  }

  if (m_data_type != DataType::CATEGORICAL) {
    m_numeric_x_data.clear();
    m_y_data.clear();
  }

  m_data_type = DataType::CATEGORICAL;
  m_categorical_x_data = x_data;
  m_y_data.emplace_back(DataSeries{y_data, color});
}

void BarPlot::Plot(const std::vector<Real>& y_data, const Color& color) {
  if (m_y_data.size() == 0) {
    m_num_bars = y_data.size();
  } else {
    if (y_data.size() != m_num_bars) {
      return;
    }
  }

  m_y_data.emplace_back(DataSeries{y_data, color});
}

void BarPlot::SetXData(const std::vector<Real>& x_data) {
  m_numeric_x_data = x_data;
  m_data_type = DataType::NUMERIC;
}

void BarPlot::SetXData(const std::vector<std::string>& x_data) {
  m_categorical_x_data = x_data;
  m_data_type = DataType::CATEGORICAL;
}

void BarPlot::SetBaseline(Real baseline) {
  m_baselines = std::vector<Real>(m_num_bars, baseline);
}

void BarPlot::SetBaselines(const std::vector<Real>& baselines) {
  m_baselines = baselines;
}

void BarPlot::SetLegend(const std::vector<std::string>& labels) {
  if (labels.empty()) {
    m_legend_labels.clear();
    return;
  }

  m_legend_labels = labels;
}

}  // namespace plotcpp
