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

#include "fonts.hpp"

namespace plotcpp {

const std::string BarPlotBase::FRAME_RECT_CLIP_PATH_ID = {"rect-clip-path"};

void BarPlotBase::SetXLabel(const std::string& label) { m_x_label = label; }

void BarPlotBase::SetYLabel(const std::string& label) { m_y_label = label; }

void BarPlotBase::Clear() {
  m_baselines.clear();
  m_numeric_x_data.clear();
  m_categorical_x_data.clear();
  m_y_data.clear();

  m_x_label.clear();
  m_y_label.clear();
}

void BarPlotBase::Build() {
  m_svg.Reset();
  m_svg.SetSize(m_width, m_height);

  CalculateFrame();

  DrawBackground();
  DrawTitle();
  DrawFrame();
}

void BarPlotBase::CalculateFrame() {
  // Frame rectangle
  m_frame_x = static_cast<float>(m_width) * FRAME_LEFT_MARGIN_REL;
  m_frame_y = static_cast<float>(m_height) * (FRAME_TOP_MARGIN_REL);
  m_frame_w = static_cast<float>(m_width) *
              (1.0f - FRAME_LEFT_MARGIN_REL - FRAME_RIGHT_MARGIN_REL);
  m_frame_h = static_cast<float>(m_height) *
              (1.0f - FRAME_TOP_MARGIN_REL - FRAME_BOTTOM_MARGIN_REL);
}

void BarPlotBase::DrawBackground() { m_svg.DrawBackground(BACKGROUND_COLOR); }

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
