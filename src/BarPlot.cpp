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

namespace plotcpp {

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
