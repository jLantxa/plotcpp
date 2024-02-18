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

#ifndef _PLOTCPP_INCLUDE_BAR_PLOT_HPP_
#define _PLOTCPP_INCLUDE_BAR_PLOT_HPP_

#include <string>
#include <utility>
#include <vector>

#include "BarPlotBase.hpp"
#include "svg.hpp"
#include "utility.hpp"

namespace plotcpp {

/**
 * @brief A bar plot.
 *
 * BarPlot supports a standard representation consisting of
 * a single data series and a stacked bar representation with multiple segments
 * stacked on top of each other.
 *
 * In both modes the baseline is 0 by default and can be changed to a custom
 * value. In standard mode the bars are drawn from the baseline value and will
 * have a weight dictated by the values provided. In stacked mode the base are
 * drawn from the baseline value and will be stacked in order they were plotted,
 * their weights determined by the values provided.
 *
 * Both modes support numeric and categorical x axis. The y axis is always
 * numeric.
 *
 * Since BarPlot can have vertical and horizontal orientations, the weights of
 * the bar are considered the y axis while the discrete categorisation of each
 * bar is represented by the x axis.
 */
class BarPlot : public BarPlotBase {
public:
  BarPlot();
  virtual ~BarPlot() = default;

  void Plot(const std::vector<Real> &x_data, const std::vector<Real> &y_data,
            const Color &color);
  void Plot(const std::vector<Real> &x_data, const std::vector<Real> &y_data);
  void Plot(const std::vector<std::string> &x_data,
            const std::vector<Real> &y_data, const Color &color);
  void Plot(const std::vector<std::string> &x_data,
            const std::vector<Real> &y_data);
  void Plot(const std::vector<Real> &y_data, const Color &color);
  void Plot(const std::vector<Real> &y_data);

  void SetXData(const std::vector<Real> &x_data);
  void SetXData(const std::vector<std::string> &x_data);

  /**
   * @brief Set a baseline value for all bars.
   * The baseline is the value from which all bar segments, positive and
   * negative will be stacked.
   *
   * @param baseline Baseline value.
   */
  void SetBaseline(Real baseline);

  /**
   * @brief Set a baseline value for every individual bar.
   * The baseline is the value from which all bar segments, positive and
   * negative will be stacked.
   *
   * @param baseline Baseline value.
   */
  void SetBaselines(const std::vector<Real> &baselines);

  /** Set the figure legend */
  void SetLegend(const std::vector<std::string> &labels);

protected:
  ColorSelector m_color_selector;
};

} // namespace plotcpp

#endif // _PLOTCPP_INCLUDE_BAR_PLOT_HPP_
