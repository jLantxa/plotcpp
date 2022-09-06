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
	for (plotcpp::Real x = 0; x < 10; x += 0.001) {
		Real y = 1/(x-5);
		data_x0.push_back(x);
		data_y0.push_back(y);
	}
	plot2d.Plot(data_x0, data_y0);

	plot2d.SetXRange(0, 10);
	plot2d.SetYRange(-10, 10);

	plot2d.Build();
	std::string svg_str = plot2d.GetSVG();

	std::cout << svg_str << std::endl;

	return 0;
}
