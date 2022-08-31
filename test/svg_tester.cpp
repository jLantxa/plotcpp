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

#include <iostream>

#include "svg.hpp"

using plotcpp::svg::Document;
using plotcpp::svg::Line;
using plotcpp::svg::Rect;
using plotcpp::svg::Path;
using plotcpp::svg::PathCommand;

int main() {
	Document svg;
	svg.SetSize(400, 300);

	Line line {
		.x1 = 0,
		.y1 = 50,
		.x2 = 100,
		.y2 = 50,
		.stroke_color = {255, 0, 0},
		.stroke_opacity = 1.0f,
		.stroke_width = 2,
	};
	svg.DrawLine(line);

	Rect rect {
		.x = 100,
		.y = 100,
		.width = 100,
		.height = 100,
		.rx = 5,
		.ry = 5,
		.stroke_color = {0, 0, 0},
		.stroke_opacity = 1.0f,
		.stroke_width = 3,
		.fill_color = {220, 160, 160},
		.fill_opacity = 0.85f
	};
	svg.DrawRect(rect);

	Path path;
	path.Add({PathCommand::Id::MOVE, 100.0f, 200.0f});
	path.Add({PathCommand::Id::LINE_R, 20.0f, 20.0f});
	path.Add({PathCommand::Id::LINE_R, 20.0f, -20.0f});
	path.Add({PathCommand::Id::LINE_R, 20.0f, 20.0f});
	path.Add({PathCommand::Id::LINE_R, 20.0f, -20.0f});
	svg.DrawPath(path);

	std::cout << svg.GetText();

	return 0;
}
