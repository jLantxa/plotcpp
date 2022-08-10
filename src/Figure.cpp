/*
 * plotcpp is a 2D plotting library for modern C++
 * Copyright (C) 2022  Javier Lancha Vázquez <javier.lancha@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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

}
