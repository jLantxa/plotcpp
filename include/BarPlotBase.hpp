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

#ifndef _PLOTCPP_INCLUDE_BAR_PLOT_BASE_HPP_
#define _PLOTCPP_INCLUDE_BAR_PLOT_BASE_HPP_

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "Figure.hpp"
#include "utility.hpp"

namespace plotcpp {

class BarPlotBase : public Figure {
 public:
  virtual ~BarPlotBase() = default;

  void Clear() override;
  void Build() override;

  void SetXLabel(const std::string& label);
  void SetYLabel(const std::string& label);

  void SetGridEnable(bool enable);

  void SetRoundedEdges(bool enable);

  void SetBarRelativeWidth(float rel_width);

  void AddYMarker(Real marker);

  void SetLegend(const std::vector<std::string>& labels);

 protected:
  explicit BarPlotBase() = default;

  void ClearData();

  enum class DataType {
    NUMERIC,
    CATEGORICAL,
  };
  DataType m_data_type = DataType::NUMERIC;

  struct DataSeries {
    std::vector<Real> values;
    Color color;
  };

  std::size_t m_num_bars;
  std::vector<Real> m_baselines;
  std::vector<Real> m_numeric_x_data;
  std::vector<std::string> m_categorical_x_data;
  std::vector<DataSeries> m_y_data;
  std::vector<std::string> m_legend_labels;

  std::string m_x_label;
  std::string m_y_label;

  float m_frame_x, m_frame_y, m_frame_w, m_frame_h;

  std::pair<Real, Real> m_y_range;

  std::set<Real> m_y_markers;
  std::set<Real> m_y_custom_markers;
  bool m_round_y_markers = false;

  bool m_grid_enable = false;
  bool m_rounded_borders = true;

  // Constraints
  static constexpr float FRAME_TOP_MARGIN_REL = 0.10f;
  static constexpr float FRAME_BOTTOM_MARGIN_REL = 0.12f;
  static constexpr float FRAME_LEFT_MARGIN_REL = 0.15f;
  static constexpr float FRAME_RIGHT_MARGIN_REL = 0.05f;
  static constexpr float BAR_FRAME_Y_MARGIN_REL = 0.05f;
  static constexpr float BAR_FRAME_X_MARGIN_REL = 0.05f;
  static constexpr float DEFAULT_BAR_WIDTH_REL = 0.65f;
  static const std::string FRAME_RECT_CLIP_PATH_ID;

  float m_bar_width_rel = DEFAULT_BAR_WIDTH_REL;

  static constexpr Color BACKGROUND_COLOR = {255, 255, 255};

  float m_axis_font_size = 11.0f;

  static constexpr float PIXELS_PER_X_MARKER = 80.0f;
  static constexpr float PIXELS_PER_Y_MARKER = 80.0f;
  static constexpr float MARKER_LENGTH = 5.0f;
  static constexpr unsigned int MAX_NUM_Y_MARKERS = 5;
  static constexpr unsigned int MAX_NUM_X_MARKERS = 10;

  static constexpr float BASE_TITLE_FONT_SIZE = 20.0f;
  static constexpr float BASE_AXIS_FONT_SIZE = 11.0f;

  static constexpr float LEGEND_MARGIN = 5.0f;

  float TranslateToFrame(Real y) const;

  float m_zoom_y = 1.0f;
  float m_bar_top_y;

  bool m_discrete_x_axis = true;

  /** Calculate all frame parameters needed to draw the plots. */
  void CalculateFrame();

  void DrawBackground();
  void DrawFrame();

  void DrawTitle();

  void DrawBars();

  void DrawXAxis();
  void DrawYAxis();
  void DrawLabels();

  void DrawLegend();
};

}  // namespace plotcpp

#endif  // _PLOTCPP_INCLUDE_BAR_PLOT_BASE_HPP_
