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

#include <algorithm>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "Figure.hpp"

namespace plotcpp {

/**
 * \brief A 2D plot consisting of one or more 2D sets of points
 *
 * @tparam X x-axis data type
 * @tparam Y y-axis data type
 */
template <typename X, typename Y>
class Plot2D : public Figure {
public:
    Plot2D() : Figure() {}

    /**
     * \brief Add a plot consisting of an x-axis sequence and a y-axis sequence
     * of the same length
     *
     * \param x_data x-axis data
     * \param y_data y-axis data
     */
    void Plot(std::vector<X> x_data, std::vector<Y> y_data);

    /**
     * \brief Add a plot consisting of one y-axis sequence of size N. The x-axis
     * sequence will be automatically deduced as an 1-increment sequence from
     * 0 to N-1.
     *
     * \param x_data x-axis data
     * \param y_data y-axis data
     */
    void Plot(std::vector<Y> y_data);

    /**
     * \brief Set a range for the x axis
     *
     * \param x0 Minimum value
     * \param x1 Maximum value
     */
    void SetXRange(X x0, X x1) {
        const X x_min = std::min(static_cast<Real>(x0), static_cast<Real>(x1));
        const X x_max = std::max(static_cast<Real>(x0), static_cast<Real>(x1));
        m_x_range = {x_min, x_max};
    }

    /**
     * \brief Set a range for the y axis
     *
     * \param y0 Minimum value
     * \param y1 Maximum value
     */
    void SetYRange(Y y0, Y y1) {
        const Y y_min = std::min(static_cast<Real>(y0), static_cast<Real>(y1));
        const Y y_max = std::max(static_cast<Real>(y0), static_cast<Real>(y1));
        m_y_range = {y_min, y_max};
    }


	/**
	 * Returns the x axis range.
	 */
	auto GetXRange() const {
		return m_x_range;
	}

	/**
	 * Returns the y axis range.
	 */
	auto GetYRange() const {
		return m_y_range;
	}


    /**
     * \brief Set a label for the x axis.
     * \param label
     */
    void SetXLabel(const std::string& label) {
        m_x_label = label;
    }

    /**
     * \brief Set a label for the y axis.
     * \param label
     */
    void SetYLabel(const std::string& label) {
        m_y_label = label;
    }

	/**
	 * \brief Returns the x axis label
	 * \return x axis label
	 */
	std::string GetXLabel() const {
		return m_x_label;
	}

	/**
	 * \brief Returns the y axis label
	 * \return y axis label
	 */
	std::string GetYLabel() const {
		return m_y_label;
	}


    /**
     * \brief Build the figure
     */
    void Build() override {
        /* TODO(jLantxa):
         * Generate graphic primitives from bottom to top:
         * 1. Add title (if defined)
         * 2. Add labels (if defined). [Y label must be translated and rotated]
         * 3. Add grid (if set)
         * 4. Calculate ranges for the largest x and y scales
         *   3.0. If N == 1: place point in the middle
         *   3.1. Establish logarithmic range of data
         *   3.2. Determine extreme integers
         *      3.2.1. If range not set, set a default margin and draw data from the margins
         *      3.2.2. If range set, y range leaves no margin, but the integer scale remains in place
         *   3.3. Subdivide integer scale according to available space
         *   3.4. Place markers in scale and order of magnitude
         * 5. Add point path for each data pair (with different colours according to list)
         * 6. Add legend (if set)
         * 7. Generate SVG description from primitives
        */
    }

protected:
    struct DataPair {
        std::vector<X> x;
        std::vector<Y> y;
    };

    std::string m_x_label;
    std::string m_y_label;

    std::vector<DataPair> m_data;
    std::optional<std::pair<Real, Real>> m_x_range;
    std::optional<std::pair<Real, Real>> m_y_range;
};

}
