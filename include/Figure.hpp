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

#include <string>

namespace plotcpp {

class Figure {
public:
	/**
	 * Render and save the figure to a file.
	 *
	 * \param filepath Path to a file to save the figure.
	 */
	virtual void Save(const std::string& filepath) const = 0;

	/**
	 * Render the figure on a window.
	 */
	virtual void Show() = 0;

	/**
	 * Set figure size in pixels.
	 *
	 * \param width
	 * \param height
	 */
	virtual void SetSize(unsigned int width, unsigned int height) = 0;

	/**
	 * Returns the figure width in pixels.
	 *
	 * \return width
	 */
	unsigned int Width() const {
		return m_width;
	}

	/**
	 * Returns the figure height in pixels.
	 *
	 * \return height
	 */
	unsigned int Height() const {
		return m_height;
	}

	/**
	 * Set figure title.
	 *
	 * \param title
	 */
	void SetTitle(const std::string& title) {
		m_title = title;
	}

	/**
	 * Returns the figure's title.
	 *
	 * \return title
	 */
	std::string Title() const {
		return m_title;
	}

protected:
	unsigned int m_figure_number;
	std::string m_title;

	static constexpr unsigned int DEFAULT_WIDTH = 400;
	static constexpr unsigned int DEFAULT_HEIGHT = 300;

	unsigned int m_width = DEFAULT_WIDTH;
	unsigned int m_height = DEFAULT_HEIGHT;

	Figure();

private:
	static unsigned int m_figure_counter;
};

}
