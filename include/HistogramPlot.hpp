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

#ifndef _PLOTCPP_INCLUDE_HISTOGRAM_PLOT_HPP_
#define _PLOTCPP_INCLUDE_HISTOGRAM_PLOT_HPP_

#include <vector>

#include "BarPlot.hpp"
#include "Figure.hpp"
#include "utility.hpp"

namespace plotcpp {

class HistogramPlot : protected BarPlotBase {
 public:
  explicit HistogramPlot() = default;
  virtual ~HistogramPlot() = default;

  /**
   * @brief Plot a histogram of a sequence of values
   *
   * @param values Vector of values
   * @param num_bins Number of bins
   * @param color Bar colour
   */
  void Plot(const std::vector<Real>& values, unsigned int num_bins,
            const Color& color);

  void SetXLabel(const std::string& label);
  void SetYLabel(const std::string& label);
};

}  // namespace plotcpp

#endif  // _PLOTCPP_INCLUDE_HISTOGRAM_PLOT_HPP_
