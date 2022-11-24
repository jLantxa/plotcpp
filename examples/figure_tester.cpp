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

#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "BarPlot.hpp"
#include "GroupFigure.hpp"
#include "HistogramPlot.hpp"
#include "Plot2D.hpp"
#include "utility.hpp"
#include "version.hpp"

using plotcpp::Real;

std::random_device rd;
std::mt19937 gen(rd());

static void NumericPlot() {
  static const std::string NUMERIC_PLOT_FILENAME{"numeric_plot2d.svg"};
  plotcpp::Plot2D plot2d;

  plot2d.SetSize(600, 450);
  plot2d.SetTitle("Plot2D");
  plot2d.SetXLabel("x axis");
  plot2d.SetYLabel("y axis");

  std::vector<Real> data_x0 =
      plotcpp::ranges::MakeRange<Real>(0.0f, 10.0f, 0.001f);
  plot2d.Plot(
      data_x0, [](auto x) { return std::sin(2 * M_PI * x) * exp(-x); },
      {0, 0, 0}, 3, "");

  std::vector<Real> data_x1 =
      plotcpp::ranges::MakeRange<Real>(0.0f, 10.0f, 0.01f);
  std::vector<Real> data_y1;
  for (auto& x : data_x1) {
    Real y = 1 / x;
    data_y1.push_back(y);
  }
  plot2d.Plot(data_x1, data_y1, {255, 32, 32}, 2, "10,5,2,5");

  std::vector<Real> data_x2 =
      plotcpp::ranges::MakeRange<Real>(10.0f, 0.0f, -0.01f);
  std::vector<Real> data_y2;
  for (auto& x : data_x2) {
    Real y = std::pow(x - 5, 3);
    data_y2.push_back(y);
  }
  plot2d.Plot(data_x2, data_y2, {32, 255, 32}, 2, "5,5");

  std::vector<Real> scatter_x;
  std::vector<Real> scatter_y;
  const Real scatter_x0 = 4.0f;
  const Real scatter_y0 = 2.71f;
  static std::normal_distribution<Real> distr(0.0f, 1.0f);
  for (std::size_t i = 0; i < 11; ++i) {
    scatter_x.push_back(scatter_x0 + distr(gen));
    scatter_y.push_back(scatter_y0 + distr(gen));
  }
  plot2d.Scatter(scatter_x, scatter_y, {64, 64, 192}, 5);

  plot2d.SetGrid(true);
  plot2d.SetYRange(-5.0f, 5.0f);
  plot2d.AddXMarker(3.14f);
  plot2d.AddXMarker(-1.0f);
  plot2d.AddYMarker(2.71f);
  plot2d.AddYMarker(5.10f);

  plot2d.SetLegend({"Exp sine", "Hyperbola", "Cubic", "Scatter"});

  plot2d.Build();
  plot2d.Save(NUMERIC_PLOT_FILENAME);
}

void CategoricalPlot() {
  static const std::string CATEGORICAL_PLOT_FILENAME{"categorical_plot2d.svg"};

  plotcpp::Plot2D plot2d;
  const std::vector<std::string> x_annotated_data{"Cat. 1", "Cat. 2", "Cat. 3"};
  const std::vector<Real> y0_annotated_data{-1, 1, 5};
  const std::vector<Real> y1_annotated_data{0, 2, 4};

  plot2d.Plot(x_annotated_data, y0_annotated_data, {128, 128, 255}, 2, "");
  plot2d.Plot(y1_annotated_data, {255, 128, 128}, 2, "");

  plot2d.SetSize(600, 450);
  plot2d.SetTitle("Categorical Plot2D");
  plot2d.SetXLabel("Categories");
  plot2d.SetYLabel("y axis");
  plot2d.SetGrid(true);
  plot2d.SetLegend({"Plot 1", "Plot 2"});

  plot2d.Build();
  plot2d.Save(CATEGORICAL_PLOT_FILENAME);
}

void GroupPlot() {
  static const std::string GROUP_PLOT_FILENAME = "group.svg";

  plotcpp::GroupFigure<2, 1> group;
  plotcpp::BarPlot p0;
  plotcpp::Plot2D p1;

  p0.Plot({2, 4, 6, 8, 10, 7, -5, -3, -1}, {41, 52, 98});
  p0.Plot({1, 2, 3, 4, 5, 6, 7, 8, 9}, {214, 28, 78});
  p0.Plot({-5, -4, -3, -2, -1, 1, 2, 3, 4}, {254, 177, 57});
  p0.SetXLabel("x axis");
  p0.SetYLabel("y axis");
  p0.SetGrid(true);
  p0.SetLegend({"S0", "S1", "S2"});

  p1.SetHold(true);
  const auto x1 = plotcpp::ranges::MakeRange<Real>(1, 6, 0.01);
  p1.Plot(
      x1, [](Real x) { return x * x; }, {255, 0, 0}, 2, "");
  p1.Plot(
      x1, [](Real x) { return x; }, {0, 255, 0}, 2, "");
  p1.Plot(
      x1, [](Real x) { return std::log2(x); }, {0, 0, 255}, 2, "");
  p1.SetXLabel("x");
  p1.SetYLabel("y");
  p1.SetGrid(true);
  p1.SetLegend({"x^2", "x", "log2(x)"});

  group.Subplot(&p0, 0, 0);
  group.Subplot(&p1, 1, 0);
  group.Build();
  group.Save(GROUP_PLOT_FILENAME);
}

void BarPlot() {
  static const std::string BAR_PLOT_FILENAME = "bar_plot.svg";
  plotcpp::BarPlot plot;

  plot.Plot({2, 4, 6, 8, 10, 7, -5, -3, -1}, {41, 52, 98});
  plot.Plot({1, 2, 3, 4, 5, 6, 7, 8, 9}, {214, 28, 78});
  plot.Plot({-5, -4, -3, -2, -1, 1, 2, 3, 4}, {254, 177, 57});

  plot.SetTitle("BarPlot");
  plot.SetXLabel("x axis");
  plot.SetYLabel("y axis");
  plot.SetGrid(true);
  plot.SetLegend({"S0", "S1", "S2"});

  plot.Build();
  plot.Save(BAR_PLOT_FILENAME);
}

void HistograpPlot() {
  static const std::string HISTOGRAM_PLOT_FILENAME = "histogram_plot.svg";
  plotcpp::HistogramPlot plot;
  static constexpr plotcpp::Color color = {41, 52, 98};
  static constexpr unsigned int num_bins = 64;
  static constexpr unsigned int num_values = 4096;

  std::vector<Real> values;
  values.resize(num_values);

  static std::normal_distribution<Real> distr(0.0f, 32.0f);
  for (std::size_t i = 0; i < num_values; ++i) {
    values[i] = distr(gen);
  }

  plot.Plot(values, num_bins, color);

  plot.SetTitle("HistogramPlot");
  plot.SetXLabel("values");
  plot.SetYLabel("Counts");
  plot.SetGrid(true);

  plot.Build();
  plot.Save(HISTOGRAM_PLOT_FILENAME);
}

int main() {
  NumericPlot();
  CategoricalPlot();
  GroupPlot();
  BarPlot();
  HistograpPlot();

  return 0;
}
