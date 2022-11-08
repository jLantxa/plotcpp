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
#include <vector>

#include "Figure.hpp"
#include "svg.hpp"
#include "utility.hpp"

namespace plotcpp {

class BarPlotBase : public Figure {
 public:
  virtual ~BarPlotBase() = default;
  void Build() override;

 protected:
  explicit BarPlotBase() = default;
};

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
  explicit BarPlot() = default;
  virtual ~BarPlot() = default;

  void PlotBars(const std::vector<Real>& x_data,
                const std::vector<Real>& y_data, const Color& color);
  void PlotBars(const std::vector<std::string>& x_data,
                const std::vector<Real>& y_data, const Color& color);
  void PlotBars(const std::vector<Real>& y_data, const Color& color);

  void StackBars(const std::vector<Real>& x_data,
                 const std::vector<Real>& y_data, const Color& color);
  void StackBars(const std::vector<std::string>& x_data,
                 const std::vector<Real>& y_data, const Color& color);
  void StackBars(const std::vector<Real>& y_data, const Color& color);

  void SetBaselines(const std::vector<Real>& baselines);

  void SetXLabel(const std::string& label);
  void SetYLabel(const std::string& label);
  void SetLegend(const std::vector<std::string>& labels);
};

}  // namespace plotcpp

#endif  // _PLOTCPP_INCLUDE_BAR_PLOT_HPP_
