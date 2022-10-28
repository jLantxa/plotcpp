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

#ifndef _PLOTCPP_INCLUDE_PLOT2D_HPP_
#define _PLOTCPP_INCLUDE_PLOT2D_HPP_

#include <algorithm>
#include <array>
#include <functional>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "Figure.hpp"
#include "ranges.hpp"

namespace plotcpp {

/**
 * \brief A 2D plot consisting of one or more 2D sets of points
 */
class Plot2D : public Figure {
 public:
  using Color = svg::RGB;

  struct Style {
    Color color;
    float stroke;
    std::string dash_array;
  };

  Plot2D();

  /**
   * \brief Add a plot consisting of an x-axis sequence and a y-axis sequence
   * of the same length
   *
   * \param x_data x-axis data
   * \param y_data y-axis data
   * \param Style style
   */
  void Plot(const std::vector<Real>& x_data, const std::vector<Real>& y_data,
            const Style& style);

  /**
   * \brief Add a plot consisting of one y-axis sequence of size N. The x-axis
   * sequence will be automatically deduced as an 1-increment sequence from
   * 0 to N-1.
   *
   * \param x_data x-axis data
   * \param Style style
   */
  void Plot(const std::vector<Real>& y_data, const Style& style);

  /**
   * \brief Add a plot using a vector as x axis values and a lambda function
   * such that y=function(x) \param x x values \param function A function such
   * that y=function(x) \param style A style for the plot
   */
  void Plot(const std::vector<Real>& x_data,
            const std::function<Real(Real)>& function, const Style& style);

  /**
   * \brief Add an categorical plot with discrete text labels on the x axis and
   * Real numbers on the y axis.
   *
   * \param x_data categorical x
   * \param y_data y-axis data axis
   * \param style style
   */
  void Plot(const std::vector<std::string>& x_data,
            const std::vector<Real>& y_data, const Style& style);

  /**
   * \brief Set hold on/off
   * Setting the hold on allows multiple data series to be plotted. If hold is
   * off, plotting overwrites the plot data.
   */
  void SetHold(bool hold_on);

  /** Enable / disable the grid */
  void SetGrid(bool enable);

  /**
   * \brief Set a range for the x axis
   *
   * \param x0 Minimum value
   * \param x1 Maximum value
   */
  void SetXRange(Real x0, Real x1);

  /**
   * \brief Set a range for the y axis
   *
   * \param y0 Minimum value
   * \param y1 Maximum value
   */
  void SetYRange(Real y0, Real y1);

  /** Returns the x axis range. */
  std::optional<ranges::Interval> GetXRange() const;

  /** Returns the x axis range. */
  std::optional<ranges::Interval> GetYRange() const;

  /** Adds a custom marker to the x axis */
  void AddXMarker(Real x);

  /** Adds a custom marker to the y axis */
  void AddYMarker(Real y);

  /** Clears all custom markers */
  void ClearMarkers();

  void SetLegend(const std::vector<std::string>& labels);

  void ClearLegend();

  /**
   * \brief Set a label for the x axis.
   * \param label
   */
  void SetXLabel(const std::string& label);

  /**
   * \brief Set a label for the y axis.
   * \param label
   */
  void SetYLabel(const std::string& label);

  /**
   * \brief Returns the x axis label
   * \return x axis label
   */
  std::string GetXLabel() const;

  /**
   * \brief Returns the y axis label
   * \return y axis label
   */
  std::string GetYLabel() const;

  /** Clear figure configuration */
  void Clear();

  /** Clear plot data */
  void ClearData();

  /**
   * \brief Build the figure
   */
  void Build() override;

 protected:
  struct DataSeries {
    std::vector<Real> x;
    std::vector<Real> y;
    Style style;
    std::string label;
  };

  struct CategoricalDataSeries {
    std::vector<std::string> x;
    std::vector<Real> y;
    Style style;

    void Clear() {
      x.clear();
      y.clear();
      style = {};
    }
  };

  bool m_hold = true;

  std::string m_x_label;
  std::string m_y_label;

  enum class DataType {
    NUMERIC,
    CATEGORICAL,
  };
  DataType m_data_type = DataType::NUMERIC;

  // Numeric data series
  std::vector<DataSeries> m_numeric_data;
  ranges::Interval m_x_data_range, m_y_data_range;
  std::optional<ranges::Interval> m_x_set_range, m_y_set_range;
  ranges::Interval m_x_range, m_y_range;
  float m_zoom_x = 1.0f;
  float m_zoom_y = 1.0f;

  // Categorical data series
  CategoricalDataSeries m_categorical_data;

  float m_frame_x, m_frame_y, m_frame_w, m_frame_h;

  std::pair<ranges::Interval, ranges::Interval> XDataRange() const;
  std::pair<ranges::Interval, ranges::Interval> YDataRange() const;

  std::set<Real> m_x_markers;
  std::set<Real> m_y_markers;
  std::set<Real> m_x_custom_markers;
  std::set<Real> m_y_custom_markers;

  bool m_grid_enable = false;
  bool m_legend_enable = false;

  /** Translate the (x, y) coordinates from the plot function to (x, y) in the
   * svg image */
  std::pair<float, float> TranslateToFrame(Real x, Real y) const;

  // Constraints
  static constexpr float FRAME_TOP_MARGIN_REL = 0.10f;
  static constexpr float FRAME_BOTTOM_MARGIN_REL = 0.12f;
  static constexpr float FRAME_LEFT_MARGIN_REL = 0.12f;
  static constexpr float FRAME_RIGHT_MARGIN_REL = 0.05f;
  static const std::string FRAME_RECT_CLIP_PATH_ID;

  static constexpr Color FRAME_STROKE_COLOR = {128, 128, 128};
  static constexpr Color BACKGROUND_COLOR = {255, 255, 255};

  const std::string TEXT_FONT{"monospace"};

  static constexpr float MARKER_LENGTH = 5.0f;
  static constexpr unsigned int MAX_NUM_Y_MARKERS = 5;
  static constexpr unsigned int MAX_NUM_X_MARKERS = 10;

  /** Calculate all frame parameters needed to draw the plots. */
  void CalculateFrame();
  void CalculateNumericFrame();
  void CalculateCategoricalFrame();

  void DrawBackground();
  void DrawFrame();

  void DrawData();
  void DrawNumericData();
  void DrawCategoricalData();

  void DrawTitle();

  void DrawAxes();
  void DrawNumericXAxis();
  void DrawCategoricalXAxis();
  void DrawYAxis();

  void DrawXLabel();
  void DrawYLabel();

  void DrawLegend();
};

}  // namespace plotcpp

#endif  // _PLOTCPP_INCLUDE_PLOT2D_HPP_
