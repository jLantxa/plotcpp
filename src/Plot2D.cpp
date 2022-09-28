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

#include "Plot2D.hpp"

#include <algorithm>
#include <limits>
#include <numeric>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "svg.hpp"

namespace plotcpp {

template <typename T>
static bool IsInfinity(T x) {
  return (x == std::numeric_limits<T>::infinity()) ||
         (x == -std::numeric_limits<T>::infinity());
}

const std::string Plot2D::FRAME_RECT_CLIP_PATH_ID = {"rect-clip-path"};

Plot2D::Plot2D() : Figure() {}

void Plot2D::Plot(const std::vector<Real>& x_data,
                  const std::vector<Real>& y_data, const Style& style) {
  const std::size_t size = x_data.size();

  if (x_data.size() != y_data.size()) {
    return;
  }

  if (m_hold == false) {
    m_data.clear();
  }

  DataSeries data_series;
  data_series.x.resize(size);
  data_series.y.resize(size);
  data_series.style = style;
  std::copy(x_data.begin(), x_data.end(), data_series.x.begin());
  std::copy(y_data.begin(), y_data.end(), data_series.y.begin());
  m_data.emplace_back(data_series);
}

void Plot2D::Plot(const std::vector<Real>& y_data, const Style& style) {
  std::vector<Real> x_data;
  x_data.resize(y_data.size());

  std::iota(x_data.begin(), x_data.end(), 1.0f);
  Plot(x_data, y_data, style);
}

void Plot2D::SetXRange(Real x0, Real x1) {
  const Real x_min = std::min(x0, x1);
  const Real x_max = std::max(x0, x1);
  m_x_set_range = {x_min, x_max};
}

void Plot2D::SetYRange(Real y0, Real y1) {
  const Real y_min = std::min(y0, y1);
  const Real y_max = std::max(y0, y1);
  m_y_set_range = {y_min, y_max};
}

std::optional<Plot2D::Range> Plot2D::GetXRange() const { return m_x_set_range; }

std::optional<Plot2D::Range> Plot2D::GetYRange() const { return m_y_set_range; }

void Plot2D::SetXLabel(const std::string& label) { m_x_label = label; }

void Plot2D::SetYLabel(const std::string& label) { m_y_label = label; }

std::string Plot2D::GetXLabel() const { return m_x_label; }

std::string Plot2D::GetYLabel() const { return m_y_label; }

void Plot2D::SetHold(bool hold) { m_hold = hold; }

void Plot2D::Clear() {
  ClearData();
  SetTitle("");
  SetSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
  SetXLabel("");
  SetYLabel("");
  m_x_set_range.reset();
  m_y_set_range.reset();
  m_svg.Reset();
}

void Plot2D::ClearData() { m_data.clear(); }

void Plot2D::Build() {
  m_svg.Reset();
  m_svg.SetSize(m_width, m_height);

  CalculateFrame();

  DrawBackground();
  DrawData();
  DrawFrame();
  DrawTitle();
  DrawXLabel();
  DrawYLabel();
}

void Plot2D::CalculateFrame() {
  // Frame rectangle
  m_frame_x = static_cast<float>(m_width) * FRAME_LEFT_MARGIN_REL;
  m_frame_y = static_cast<float>(m_height) * (FRAME_TOP_MARGIN_REL);
  m_frame_w = static_cast<float>(m_width) *
              (1.0f - FRAME_LEFT_MARGIN_REL - FRAME_RIGHT_MARGIN_REL);
  m_frame_h = static_cast<float>(m_height) *
              (1.0f - FRAME_TOP_MARGIN_REL - FRAME_BOTTOM_MARGIN_REL);

  // Ranges
  Real min_x = std::numeric_limits<Real>::max();
  Real max_x = std::numeric_limits<Real>::lowest();
  Real min_y = std::numeric_limits<Real>::max();
  Real max_y = std::numeric_limits<Real>::lowest();
  for (auto& plot : m_data) {
    const std::size_t size = plot.x.size();
    for (std::size_t i = 0; i < size; ++i) {
      const Real x = plot.x[i];
      const Real y = plot.y[i];

      if (!IsInfinity(x)) {
        min_x = std::min(x, min_x);
        max_x = std::max(x, max_x);
      }
      if (!IsInfinity(y)) {
        min_y = std::min(y, min_y);
        max_y = std::max(y, max_y);
      }
    }
  }
  m_x_data_range = {min_x, max_x};
  m_y_data_range = {min_y, max_y};

  m_x_range =
      m_x_set_range.has_value() ? m_x_set_range.value() : m_x_data_range;
  m_y_range =
      m_y_set_range.has_value() ? m_y_set_range.value() : m_y_data_range;

  // Zoom factors
  m_zoom_x =
      static_cast<float>(abs(m_frame_w / (m_x_range.second - m_x_range.first)));
  m_zoom_y =
      static_cast<float>(abs(m_frame_h / (m_y_range.second - m_y_range.first)));
}

std::pair<float, float> Plot2D::TranslateToFrame(Real x, Real y) const {
  float tx = static_cast<float>((m_zoom_x * (x - m_x_range.first)) + m_frame_x);
  float ty = static_cast<float>(-(m_zoom_y * (y - m_y_range.first)) +
                                (m_frame_y + m_frame_h));
  return {tx, ty};
}

void Plot2D::DrawBackground() { m_svg.DrawBackground(BACKGROUND_COLOR); }

void Plot2D::DrawFrame() {
  svg::Rect frame_rect{
      .x = m_frame_x,
      .y = m_frame_y,
      .width = m_frame_w,
      .height = m_frame_h,
      .stroke_color = FRAME_STROKE_COLOR,
      .fill_opacity = 0.0f,
  };
  m_svg.DrawRect(frame_rect);

  auto defs_node = m_svg.Defs();
  auto clip_path_node = svg::AppendNode(defs_node, "clipPath");
  svg::SetAttribute(clip_path_node, "id", FRAME_RECT_CLIP_PATH_ID);
  svg::Rect clip_rect{
      .x = m_frame_x, .y = m_frame_y, .width = m_frame_w, .height = m_frame_h};
  m_svg.DrawRect(clip_rect, clip_path_node);
}

void Plot2D::DrawData() {
  const std::size_t num_plots = m_data.size();
  for (std::size_t p = 0; p < num_plots; ++p) {
    const DataSeries& plot = m_data[p];

    svg::Path path;
    path.stroke_color = plot.style.color;
    path.stroke_width = plot.style.stroke;

    const std::vector<Real>& data_x = plot.x;
    const std::vector<Real>& data_y = plot.y;
    const std::size_t size = data_x.size();

    for (std::size_t i = 0; i < size; ++i) {
      if (IsInfinity(data_y[i])) {
        continue;
      }

      const bool must_join_points = (i > 0) && !IsInfinity(data_y[i - 1]);
      const auto path_cmd = must_join_points ? svg::PathCommand::Id::LINE
                                             : svg::PathCommand::Id::MOVE;
      auto [tx, ty] = TranslateToFrame(data_x[i], data_y[i]);
      path.Add({path_cmd, tx, ty});
    }

    auto path_node = m_svg.DrawPath(path);

    std::stringstream clip_path_url_ss;
    clip_path_url_ss << "url(#" << FRAME_RECT_CLIP_PATH_ID << ")";
    svg::SetAttribute(path_node, "clip-path", clip_path_url_ss.str());

    svg::SetAttribute(path_node, "stroke-linecap", "round");
    if (!plot.style.dash_array.empty()) {
      svg::SetAttribute(path_node, "stroke-dasharray", plot.style.dash_array);
    }

    // TODO: Can further customize the paths with:
    // svg::SetAttribute(path_node, "stroke-linejoin", "bevel");
  }
}

void Plot2D::DrawTitle() {
  if (m_title.empty()) {
    return;
  }

  float x = static_cast<float>(m_width) / 2;
  float y = static_cast<float>(m_height) * FRAME_TOP_MARGIN_REL / 2;

  auto node_ptr = m_svg.DrawText(svg::Text{m_title, x, y, 20, "arial"});
  svg::SetAttribute(node_ptr, "font-weight", "bold");
  svg::SetAttribute(node_ptr, "text-anchor", "middle");
}

void Plot2D::DrawXLabel() {
  if (m_x_label.empty()) {
    return;
  }

  const float frame_bottom = m_frame_y + m_frame_h;
  float x = m_frame_x + (m_frame_w / 2);
  float y =
      frame_bottom + (0.75f) * (static_cast<float>(m_height) - frame_bottom);

  auto node_ptr = m_svg.DrawText(svg::Text{m_x_label, x, y, 12, "arial"});
  svg::SetAttribute(node_ptr, "text-anchor", "middle");
}

void Plot2D::DrawYLabel() {
  if (m_x_label.empty()) {
    return;
  }

  float x = (1 - 0.75f) * m_frame_x;
  float y = m_frame_y + (m_frame_h / 2);

  auto node_ptr = m_svg.DrawText(svg::Text{m_y_label, 0, 0, 12, "arial"});
  svg::SetAttribute(node_ptr, "text-anchor", "middle");

  std::stringstream trans_ss;
  trans_ss << "translate(" << std::to_string(x) << ", " << std::to_string(y)
           << ") "
           << "rotate(-90)";
  svg::SetAttribute(node_ptr, "transform", trans_ss.str());
}

}  // namespace plotcpp
