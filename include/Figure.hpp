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

#include "common.hpp"

namespace plotcpp {

class Figure {
public:
	/**
	 * \brief Build the figure with its current data and configuration. This sets the
	 * figure ready to be displayed or saved.
	 */
	virtual void Build() = 0;

	/**
	 * \brief Render the figure on a window.
	 */
	void Show() const;

	/**
	 * Render and save the figure to a file.
	 * \param filepath Path to a file to save the figure.
	 */
	void Save(const std::string& filepath) const;

	/**
	 * \brief Set figure title.
	 * \param title
	 */
	void SetTitle(const std::string& title);

	/**
	 * \brief Returns the figure's title.
	 * \return title
	 */
	std::string Title() const;

	/**
	 * \brief Returns the figure number.
	 */
	unsigned int GetID() const;

	/**
	 * \brief Set figure size in pixels.
	 * \param width
	 * \param height
	 */
	void SetSize(unsigned int width, unsigned int height);

	/**
	 * \brief Returns the figure width in pixels.
	 * \return width
	 */
	unsigned int Width() const;

	/**
	 * \brief Returns the figure height in pixels.
	 * \return height
	 */
	unsigned int Height() const;

protected:
	unsigned int m_figure_number;
	std::string m_title;

	static constexpr unsigned int DEFAULT_WIDTH = 400;
	static constexpr unsigned int DEFAULT_HEIGHT = 300;

	unsigned int m_width = DEFAULT_WIDTH;
	unsigned int m_height = DEFAULT_HEIGHT;

	/** String containing an XML description of an SVG image */
	std::string m_svg;

	explicit Figure();

private:
	/** Static counter of figures. The figure number can be used to identify the
	 * figure in case this has no title.
	 */
	static unsigned int m_figure_counter;
};

}
