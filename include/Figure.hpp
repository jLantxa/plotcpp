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

#ifndef _PLOTCPP_INCLUDE_FIGURE_HPP_
#define _PLOTCPP_INCLUDE_FIGURE_HPP_

#include <string>

#include "plotcpp.hpp"
#include "svg.hpp"

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
	 * \brief Render and save the figure to a file.
	 * \param filepath Path to a file to save the figure.
	 */
	void Save(const std::string& filepath) const;

	/**
	 * \brief Return the SVG representation of this Figure.
	 * This function must be called after Build
	 */
	std::string GetSVG() const;

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

	static constexpr unsigned int DEFAULT_WIDTH = 800;
	static constexpr unsigned int DEFAULT_HEIGHT = 600;

	unsigned int m_width = DEFAULT_WIDTH;
	unsigned int m_height = DEFAULT_HEIGHT;

	/** String containing an XML description of an SVG image */
	svg::Document m_svg;

	explicit Figure();

private:
	/** Static counter of figures. The figure number can be used to identify the
	 * figure in case this has no title.
	 */
	static unsigned int m_figure_counter;
};

}  // namespace plotcpp

#endif  // _PLOTCPP_INCLUDE_FIGURE_HPP_
