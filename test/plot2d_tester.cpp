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

int main() {
	Plot2D plot2d;

	plot2d.SetSize(800, 600);
	plot2d.SetTitle("Beautiful plots");
	plot2d.SetXLabel("x label");
	plot2d.SetYLabel("y label");

	std::vector<Real> data_x0;
	std::vector<Real> data_y0;
	for (plotcpp::Real x = 0; x < 10*M_PI; x += 0.01) {
		Real y = std::sin(2*x) * std::exp(-x/10.0f);
		data_x0.push_back(x);
		data_y0.push_back(y);
	}
	plot2d.Plot(data_x0, data_y0);

	std::vector<Real> data_x1;
	std::vector<Real> data_y1;
	for (plotcpp::Real x = 0; x < 10*M_PI; x += 0.01) {
		Real y = std::cos(x);
		data_x1.push_back(x);
		data_y1.push_back(y);
	}
	plot2d.Plot(data_x1, data_y1, {
		.color={0, 128, 0},
		.dash_array="10, 5, 2, 5"});

	std::vector<Real> data_x2;
	std::vector<Real> data_y2;
	for (plotcpp::Real x = 0; x < 10*M_PI; x += 0.01) {
		Real y = 2;
		data_x2.push_back(x);
		data_y2.push_back(y);
	}
	plot2d.Plot(data_x2, data_y2, {{0, 0, 255}, "6, 2"});

	std::vector<Real> data_x3;
	std::vector<Real> data_y3;
	for (plotcpp::Real x = 0; x < 10*M_PI; x += 0.01) {
		Real y = -(x-1)*(x-1)/3 + 2;
		data_x3.push_back(x);
		data_y3.push_back(y);
	}
	plot2d.Plot(data_x3, data_y3, {{255, 0, 0}});

	plot2d.SetXRange(0, 10*M_PI);
	plot2d.SetYRange(-1.5, 3);

	plot2d.Build();
	std::string svg_str = plot2d.GetSVG();

	std::cout << svg_str << std::endl;

	return 0;
}
