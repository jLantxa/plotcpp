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

#include <fmt/format.h>

#include <algorithm>
#include <functional>
#include <limits>
#include <numeric>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "fonts.hpp"
#include "svg.hpp"
#include "utility.hpp"

namespace plotcpp {

template <typename T>
static bool IsInfinity(T x) {
  return (x == std::numeric_limits<T>::infinity()) ||
         (x == -std::numeric_limits<T>::infinity());
}

const std::string Plot2D::FRAME_RECT_CLIP_PATH_ID = {"rect-clip-path"};

Plot2D::Plot2D() : Figure() {}

void Plot2D::Plot(const std::vector<Real>& x_data,
                  const std::vector<Real>& y_data, const Color& color,
                  const float stroke_width, const std::string& dash_array) {
  if (x_data.size() != y_data.size()) {
    return;
  }

  if (m_hold == false) {
    m_numeric_data.clear();
  }

  const Style style = {color, stroke_width, dash_array, false};
  m_numeric_data.emplace_back(DataSeries{x_data, y_data, style});

  m_data_type = DataType::NUMERIC;
  m_categorical_data.clear();
}

void Plot2D::Plot(const std::vector<Real>& y_data, const Color& color,
                  const float stroke_width, const std::string& dash_array) {
  if (m_data_type == DataType::NUMERIC) {
    std::vector<Real> x_data;
    x_data.resize(y_data.size());
    std::iota(x_data.begin(), x_data.end(), 1.0f);
    Plot(x_data, y_data, color, stroke_width, dash_array);
  } else if (m_data_type == DataType::CATEGORICAL) {
    if ((m_categorical_labels.size() > 0) &&
        (m_categorical_labels.size() != y_data.size())) {
      return;
    }

    const Style style = {color, stroke_width, dash_array, false};
    m_categorical_data.emplace_back(CategoricalDataSeries{y_data, style});
  }
}

void Plot2D::Plot(const std::vector<Real>& x_data,
                  const std::function<Real(Real)>& function, const Color& color,
                  const float stroke_width, const std::string& dash_array) {
  const auto y_data = ranges::Generate(x_data, function);
  Plot(x_data, y_data, color, stroke_width, dash_array);
}

void Plot2D::Plot(const std::vector<std::string>& x_data,
                  const std::vector<Real>& y_data, const Color& color,
                  const float stroke_width, const std::string& dash_array) {
  if (x_data.size() != y_data.size()) {
    return;
  }

  // Reset data if the number of labels is different
  const bool should_reset_data =
      (x_data.size() != m_categorical_labels.size()) &&
      (m_categorical_data.size() > 0);
  if (should_reset_data) {
    m_categorical_data.clear();
  }

  // Reset labels
  m_categorical_data.clear();
  m_categorical_labels = x_data;

  const Style style = {color, stroke_width, dash_array, false};
  m_categorical_data.emplace_back(CategoricalDataSeries{y_data, style});
  m_numeric_data.clear();
  m_data_type = DataType::CATEGORICAL;
}

void Plot2D::Scatter(const std::vector<Real>& x_data,
                     const std::vector<Real>& y_data, const Color& color,
                     const float radius) {
  if (x_data.size() != y_data.size()) {
    return;
  }

  if (m_hold == false) {
    m_numeric_data.clear();
  }

  const Style style = {color, radius, "", true};
  m_numeric_data.emplace_back(DataSeries{x_data, y_data, style});

  m_data_type = DataType::NUMERIC;
  m_categorical_data.clear();
}

void Plot2D::Scatter(const std::vector<std::string>& x_data,
                     const std::vector<Real>& y_data, const Color& color,
                     const float radius) {
  if (x_data.size() != y_data.size()) {
    return;
  }

  // Reset data if labels are different
  const bool labels_are_equal = (x_data == m_categorical_labels);
  if (!labels_are_equal) {
    m_categorical_data.clear();
    m_categorical_labels = x_data;
  }

  const Style style = {color, radius, "", true};
  m_categorical_data.emplace_back(CategoricalDataSeries{y_data, style});
  m_numeric_data.clear();
  m_data_type = DataType::CATEGORICAL;
}

void Plot2D::SetXRange(Real x0, Real x1) {
  const Real x_min = std::min(x0, x1);
  const Real x_max = std::max(x0, x1);
  m_x_set_range = ranges::Interval<Real>{x_min, x_max};
}

void Plot2D::SetYRange(Real y0, Real y1) {
  const Real y_min = std::min(y0, y1);
  const Real y_max = std::max(y0, y1);
  m_y_set_range = ranges::Interval<Real>{y_min, y_max};
}

std::optional<ranges::Interval<Real>> Plot2D::GetXRange() const {
  return m_x_set_range;
}

std::optional<ranges::Interval<Real>> Plot2D::GetYRange() const {
  return m_y_set_range;
}

void Plot2D::SetXLabel(const std::string& label) { m_x_label = label; }

void Plot2D::SetYLabel(const std::string& label) { m_y_label = label; }

std::string Plot2D::GetXLabel() const { return m_x_label; }

std::string Plot2D::GetYLabel() const { return m_y_label; }

void Plot2D::AddXMarker(Real x) { m_x_custom_markers.insert(x); }

void Plot2D::AddYMarker(Real y) { m_y_custom_markers.insert(y); }

void Plot2D::SetGrid(bool enable) { m_grid_enable = enable; }

void Plot2D::SetHold(bool hold) { m_hold = hold; }

void Plot2D::SetLegend(const std::vector<std::string>& labels) {
  if (labels.empty()) {
    m_legend_labels.clear();
    return;
  }

  std::size_t num_plots = 0;
  switch (m_data_type) {
    case DataType::NUMERIC:
      num_plots = m_numeric_data.size();
      break;

    case DataType::CATEGORICAL:
      num_plots = m_categorical_data.size();
      break;
  }

  const std::size_t num_labels = std::min(labels.size(), num_plots);
  m_legend_labels.resize(num_labels);
  for (std::size_t i = 0; i < num_labels; ++i) {
    m_legend_labels[i] = labels[i];
  }
}

void Plot2D::Clear() {
  ClearData();
  m_title.clear();
  SetSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
  m_x_label.clear();
  m_y_label.clear();
  m_x_set_range.reset();
  m_y_set_range.reset();
  ClearMarkers();
  m_svg.Reset();
}

void Plot2D::ClearMarkers() {
  m_x_markers.clear();
  m_y_markers.clear();
  m_x_custom_markers.clear();
  m_y_custom_markers.clear();
}

void Plot2D::ClearData() { m_numeric_data.clear(); }

void Plot2D::Build() {
  m_svg.Reset();
  m_svg.SetSize(m_width, m_height);

  CalculateFrame();

  DrawBackground();
  DrawTitle();
  DrawAxes();
  DrawData();
  DrawFrame();
  DrawLegend();
}

void Plot2D::CalculateFrame() {
  // Frame rectangle
  m_frame_x = static_cast<float>(m_width) * FRAME_LEFT_MARGIN_REL;
  m_frame_y = static_cast<float>(m_height) * (FRAME_TOP_MARGIN_REL);
  m_frame_w = static_cast<float>(m_width) *
              (1.0f - FRAME_LEFT_MARGIN_REL - FRAME_RIGHT_MARGIN_REL);
  m_frame_h = static_cast<float>(m_height) *
              (1.0f - FRAME_TOP_MARGIN_REL - FRAME_BOTTOM_MARGIN_REL);

  switch (m_data_type) {
    case DataType::NUMERIC:
      CalculateNumericFrame();
      break;
    case DataType::CATEGORICAL:
      CalculateCategoricalFrame();
      break;
    default:
      break;
  }

  // Calculate markers
  const unsigned int num_x_markers =
      std::min(MAX_NUM_X_MARKERS,
               static_cast<unsigned int>(m_frame_w / PIXELS_PER_X_MARKER));
  m_x_markers = ranges::PartitionRange(m_x_range, num_x_markers);
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
  // const float x_axis_font_size = fonts::ConstrainedFontSize(
  //     BASE_AXIS_FONT_SIZE, DUMMY_TEXT, TEXT_FONT, m_frame_w,
  //     (static_cast<float>(m_height) * FRAME_BOTTOM_MARGIN_REL -
  //     MARKER_LENGTH) /
  //         2.0f);

  m_axis_font_size = std::min({y_axis_font_size});
}

void Plot2D::CalculateNumericFrame() {
  // Ranges
  Real min_x = std::numeric_limits<Real>::max();
  Real max_x = std::numeric_limits<Real>::lowest();
  Real min_y = std::numeric_limits<Real>::max();
  Real max_y = std::numeric_limits<Real>::lowest();
  for (auto& plot : m_numeric_data) {
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

void Plot2D::CalculateCategoricalFrame() {
  // Ranges
  Real min_y = std::numeric_limits<Real>::max();
  Real max_y = std::numeric_limits<Real>::lowest();
  for (const auto& data : m_categorical_data) {
    for (const auto& y : data.y) {
      if (!IsInfinity(y)) {
        min_y = std::min(y, min_y);
        max_y = std::max(y, max_y);
      }
    }
  }
  m_y_data_range = {min_y, max_y};

  m_y_range =
      m_y_set_range.has_value() ? m_y_set_range.value() : m_y_data_range;

  // Zoom factor
  m_zoom_y =
      static_cast<float>(abs(m_frame_h / (m_y_range.second - m_y_range.first)));

  const unsigned int num_y_markers =
      std::min(MAX_NUM_Y_MARKERS,
               static_cast<unsigned int>(m_frame_h / PIXELS_PER_Y_MARKER));
  m_y_markers = ranges::PartitionRange(m_y_range, num_y_markers);
}

void Plot2D::DrawBackground() { m_svg.DrawBackground(BACKGROUND_COLOR); }

void Plot2D::DrawFrame() {
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

void Plot2D::DrawData() {
  switch (m_data_type) {
    case DataType::NUMERIC:
      DrawNumericData();
      break;
    case DataType::CATEGORICAL:
      DrawCategoricalData();
      break;
    default:
      break;
  }
}

void Plot2D::DrawNumericData() {
  for (const auto& plot : m_numeric_data) {
    if (plot.style.scatter == false) {
      DrawNumericPath(plot);
    } else {
      DrawNumericScatter(plot);
    }
  }
}

void Plot2D::DrawNumericPath(const DataSeries& plot) {
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
    const auto [tx, ty] = TranslateToFrame(data_x[i], data_y[i]);
    path.Add({path_cmd, {tx, ty}});
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

void Plot2D::DrawNumericScatter(const DataSeries& plot) {
  const std::vector<Real>& data_x = plot.x;
  const std::vector<Real>& data_y = plot.y;
  const std::size_t size = data_x.size();

  for (std::size_t i = 0; i < size; ++i) {
    if (IsInfinity(data_y[i])) {
      continue;
    }
    const auto [tx, ty] = TranslateToFrame(data_x[i], data_y[i]);

    auto circle_node =
        m_svg.DrawCircle({tx, ty, plot.style.stroke, plot.style.color});

    std::stringstream clip_path_url_ss;
    clip_path_url_ss << "url(#" << FRAME_RECT_CLIP_PATH_ID << ")";
    svg::SetAttribute(circle_node, "clip-path", clip_path_url_ss.str());
  }
}

void Plot2D::DrawCategoricalData() {
  for (const auto& plot : m_categorical_data) {
    if (plot.style.scatter == false) {
      DrawCategoricalPath(plot);
    } else {
      DrawCategoricalScatter(plot);
    }
  }
}

void Plot2D::DrawCategoricalPath(const CategoricalDataSeries& plot) {
  svg::Path path;
  path.stroke_color = plot.style.color;
  path.stroke_width = plot.style.stroke;

  const std::vector<Real>& data_y = plot.y;
  const std::size_t size = data_y.size();

  for (std::size_t i = 0; i < size; ++i) {
    if (IsInfinity(data_y[i])) {
      continue;
    }

    const bool must_join_points = (i > 0) && !IsInfinity(data_y[i - 1]);
    const auto path_cmd = must_join_points ? svg::PathCommand::Id::LINE
                                           : svg::PathCommand::Id::MOVE;
    const auto [_, ty] = TranslateToFrame(0, data_y[i]);
    const float tx = m_frame_x + static_cast<float>(i) *
                                     (m_frame_w / static_cast<float>(size - 1));
    path.Add({path_cmd, {tx, ty}});
  }

  auto path_node = m_svg.DrawPath(path);

  std::stringstream clip_path_url_ss;
  clip_path_url_ss << "url(#" << FRAME_RECT_CLIP_PATH_ID << ")";
  svg::SetAttribute(path_node, "clip-path", clip_path_url_ss.str());

  svg::SetAttribute(path_node, "stroke-linecap", "round");
  if (!plot.style.dash_array.empty()) {
    svg::SetAttribute(path_node, "stroke-dasharray", plot.style.dash_array);
  }
}

void Plot2D::DrawCategoricalScatter(const CategoricalDataSeries& plot) {
  const std::vector<Real>& data_y = plot.y;
  const std::size_t size = data_y.size();

  for (std::size_t i = 0; i < size; ++i) {
    if (IsInfinity(data_y[i])) {
      continue;
    }

    const auto [_, ty] = TranslateToFrame(0, data_y[i]);
    const float tx = m_frame_x + static_cast<float>(i) *
                                     (m_frame_w / static_cast<float>(size - 1));

    svg::Circle circle{
        .cx = tx,
        .cy = ty,
        .r = plot.style.stroke,
        .fill_color = plot.style.color,
    };
    auto circle_node = m_svg.DrawCircle(circle);

    std::stringstream clip_path_url_ss;
    clip_path_url_ss << "url(#" << FRAME_RECT_CLIP_PATH_ID << ")";
    svg::SetAttribute(circle_node, "clip-path", clip_path_url_ss.str());
  }
}

void Plot2D::DrawTitle() {
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

void Plot2D::DrawAxes() {
  switch (m_data_type) {
    case DataType::NUMERIC:
      DrawNumericXAxis();
      break;
    case DataType::CATEGORICAL:
      DrawCategoricalXAxis();
      break;
    default:
      break;
  }
  DrawXLabel();
  DrawYAxis();
  DrawYLabel();
}

void Plot2D::DrawXLabel() {
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

void Plot2D::DrawYLabel() {
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

void Plot2D::DrawNumericXAxis() {
  std::set<Real> markers;
  markers.insert(m_x_markers.begin(), m_x_markers.end());
  markers.insert(m_x_custom_markers.begin(), m_x_custom_markers.end());

  if (markers.size() <= 1) {
    // TODO: Draw marker in the middle
    return;
  }

  for (const auto& marker : markers) {
    if ((marker < m_x_range.first) || (marker > m_x_range.second)) {
      continue;
    }

    const auto [x, _] = TranslateToFrame(marker, 0);
    const auto y = m_frame_y + m_frame_h;

    svg::Line marker_line{.x1 = x,
                          .y1 = y,
                          .x2 = x,
                          .y2 = y + MARKER_LENGTH,
                          .stroke_color = FRAME_STROKE_COLOR,
                          .stroke_opacity = 1.0f,
                          .stroke_width = 1};
    m_svg.DrawLine(marker_line);

    const std::string marker_text = fmt::format("{:.2g}", marker);
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

void Plot2D::DrawCategoricalXAxis() {
  const std::size_t num_labels = m_categorical_labels.size();
  if (num_labels <= 1) {
    // TODO: Draw marker in the middle
    return;
  }

  for (std::size_t i = 0; i < num_labels; ++i) {
    const auto x =
        m_frame_x + static_cast<float>(i) *
                        (m_frame_w / static_cast<float>(num_labels - 1));
    const auto y = m_frame_y + m_frame_h;

    svg::Line marker_line{.x1 = x,
                          .y1 = y,
                          .x2 = x,
                          .y2 = y + MARKER_LENGTH,
                          .stroke_color = FRAME_STROKE_COLOR,
                          .stroke_opacity = 1.0f,
                          .stroke_width = 1};
    m_svg.DrawLine(marker_line);

    const std::string marker_text = m_categorical_labels[i];
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

void Plot2D::DrawYAxis() {
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

    const auto [_, y] = TranslateToFrame(0, marker);
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
    const std::string marker_text = fmt::format("{:.2f}", marker);
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

void Plot2D::DrawLegend() {
  if (m_legend_labels.empty()) {
    return;
  }

  static constexpr float font_size = 12.0f;
  static constexpr float box_margin_px = 5.0f;
  static constexpr float font_em = font_size / 12.0f;
  static constexpr float font_margin_em = 0.5f * font_em;
  static constexpr float dash_length_em = 2.0f * font_em;
  static constexpr float spacing_em = 0.5f * font_em;

  // Calculate sizes
  float max_font_width_em = 0;
  for (const auto& label : m_legend_labels) {
    const auto [w, _] = fonts::CalculateTextSize(label, TEXT_FONT, font_size);
    max_font_width_em = std::max(max_font_width_em, w);
  }

  std::size_t num_plots = 0;
  std::vector<Style> styles;
  switch (m_data_type) {
    case DataType::NUMERIC:
      num_plots = m_numeric_data.size();
      for (const auto& data : m_numeric_data) {
        styles.push_back(data.style);
      }
      break;
    case DataType::CATEGORICAL:
      num_plots = m_categorical_data.size();
      for (const auto& data : m_categorical_data) {
        styles.push_back(data.style);
      }
      break;
  }

  const std::size_t num_labels = std::min(m_legend_labels.size(), num_plots);
  const float box_w = fonts::EmToPx(2 * font_margin_em + dash_length_em +
                                    spacing_em + max_font_width_em);
  const float box_x = (m_frame_x + m_frame_w) - box_margin_px - box_w;
  const float box_y = m_frame_y + box_margin_px;
  const float box_h = fonts::EmToPx(static_cast<float>(num_labels) * font_em +
                                    2 * font_margin_em);

  svg::Rect box_rect = {
      .x = box_x,
      .y = box_y,
      .width = box_w,
      .height = box_h,
  };
  auto box_rect_node = m_svg.DrawRect(box_rect);
  svg::SetAttribute(box_rect_node, "fill", "white");
  svg::SetAttribute(box_rect_node, "fill-opacity", "0.9");
  svg::SetAttribute(box_rect_node, "rx", "4", "px");
  svg::SetAttribute(box_rect_node, "ry", "4", "px");

  for (std::size_t i = 0; i < num_labels; ++i) {
    const std::string& label = m_legend_labels[i];
    const Style& style = styles[i];
    const float x = box_x + fonts::EmToPx(font_margin_em);
    const float y = box_y + fonts::EmToPx(font_em / 2 + font_margin_em +
                                          static_cast<float>(i) * font_em);

    if (style.scatter == false) {
      svg::Line dash = {.x1 = x,
                        .y1 = y,
                        .x2 = x + fonts::EmToPx(dash_length_em),
                        .y2 = y,
                        .stroke_color = style.color,
                        .stroke_width = 2.0f};
      auto line_node = m_svg.DrawLine(dash);
      svg::SetAttribute(line_node, "stroke-dasharray", style.dash_array);
    } else {
      svg::Circle circle{
          .cx = x + (fonts::EmToPx(dash_length_em) / 2.0f),
          .cy = y,
          .r = fonts::EmToPx(font_em / 3.0f),
          .fill_color = style.color,
      };
      m_svg.DrawCircle(circle);
    }

    const float text_x = x + fonts::EmToPx(dash_length_em + spacing_em);
    const float text_y = y + fonts::EmToPx(font_em / 4);
    m_svg.DrawText(svg::Text{label, text_x, text_y, font_size, TEXT_FONT});
  }
}

}  // namespace plotcpp
