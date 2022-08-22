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
 */
class Plot2D : public Figure {
public:
    Plot2D();

    /**
     * \brief Add a plot consisting of an x-axis sequence and a y-axis sequence
     * of the same length
     *
     * \param x_data x-axis data
     * \param y_data y-axis data
     */
    void Plot(const std::vector<Real>& x_data, const std::vector<Real>& y_data);

    /**
     * \brief Add a plot consisting of one y-axis sequence of size N. The x-axis
     * sequence will be automatically deduced as an 1-increment sequence from
     * 0 to N-1.
     *
     * \param x_data x-axis data
     * \param y_data y-axis data
     */
    void Plot(const std::vector<Real>& y_data);

    /**
     * \brief Set a range for the x axis
     *
     * \param x0 Minimum value
     * \param x1 Maximum value
     */
    void SetXRange(Real x0, Real x1);

    /**
     * \brief Set a range for the y axis
     *
     * \param y0 Minimum value
     * \param y1 Maximum value
     */
    void SetYRange(Real y0, Real y1);

    /** Returns the x axis range. */
    std::optional<std::pair<Real, Real>> GetXRange() const;

    /** Returns the x axis range. */
    std::optional<std::pair<Real, Real>> GetYRange() const;

    /**
     * \brief Set a label for the x axis.
     * \param label
     */
    void SetXLabel(const std::string& label);

    /**
     * \brief Set a label for the y axis.
     * \param label
     */
    void SetYLabel(const std::string& label);

    /**
     * \brief Returns the x axis label
     * \return x axis label
     */
    std::string GetXLabel() const;

    /**
     * \brief Returns the y axis label
     * \return y axis label
     */
    std::string GetYLabel() const;

    /** Clear figure configuration */
    void Clear();

    /** Clear plot data */
    void ClearData();

    /**
     * \brief Build the figure
     */
    void Build() override;

protected:
    using DataPair = std::pair<std::vector<Real>, std::vector<Real>>;

    std::string m_x_label;
    std::string m_y_label;

    std::vector<DataPair> m_data;
    std::optional<std::pair<Real, Real>> m_x_range;
    std::optional<std::pair<Real, Real>> m_y_range;
};

}
