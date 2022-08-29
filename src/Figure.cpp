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

#include "Figure.hpp"

namespace plotcpp {

unsigned int Figure::m_figure_counter = 0;

Figure::Figure() {
	m_figure_number = ++m_figure_counter;
}

void Figure::SetTitle(const std::string& title) {
	m_title = title;
}

std::string Figure::Title() const {
	return m_title;
}

unsigned int Figure::GetID() const {
	return m_figure_number;
}

void Figure::SetSize(unsigned int width, unsigned int height) {
	m_width = width;
	m_height = height;
}

unsigned int Figure::Width() const {
	return m_width;
}

unsigned int Figure::Height() const {
	return m_height;
}

}  // namespace plotcpp
