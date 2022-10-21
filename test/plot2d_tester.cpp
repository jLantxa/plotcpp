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

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "Plot2D.hpp"

using plotcpp::Plot2D;
using plotcpp::Real;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Specify a path to save the plot" << std::endl;
  }

  const std::string filepath{argv[1]};

  Plot2D plot2d;

  plot2d.SetSize(800, 600);
  plot2d.SetTitle("Test plot");
  plot2d.SetXLabel("x axis");
  plot2d.SetYLabel("y axis");

  std::vector<Real> data_x0;
  std::vector<Real> data_y0;
  for (plotcpp::Real x = 0; x <= 10; x += 0.001) {
    Real y = std::sin(2 * M_PI * x) * exp(-x);
    data_x0.push_back(x);
    data_y0.push_back(y);
  }
  plot2d.Plot(data_x0, data_y0, {{0, 0, 0}, 3, ""});

  std::vector<Real> data_x1;
  std::vector<Real> data_y1;
  for (plotcpp::Real x = 0; x <= 10; x += 0.001) {
    Real y = 1 / x;
    data_x1.push_back(x);
    data_y1.push_back(y);
  }
  plot2d.Plot(data_x1, data_y1, {{255, 32, 32}, 2, "10,5,2,5"});

  std::vector<Real> data_x2;
  std::vector<Real> data_y2;
  for (plotcpp::Real x = 0; x <= 10; x += 0.001) {
    Real y = std::pow(x - 5, 3);
    data_x2.push_back(x);
    data_y2.push_back(y);
  }
  plot2d.Plot(data_x2, data_y2, {{32, 255, 32}, 2, "5,5"});

  plot2d.SetYRange(-5, 5);

  plot2d.Build();
  plot2d.Save(filepath);

  return 0;
}
