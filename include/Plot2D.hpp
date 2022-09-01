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

#ifndef _PLOTCPP_INCLUDE_PLOT2D_HPP_
#define _PLOTCPP_INCLUDE_PLOT2D_HPP_

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

	// Constraints
	static constexpr float FRAME_TOP_MARGIN_REL = 0.15f;
	static constexpr float FRAME_BOTTOM_MARGIN_REL = 0.10f;
	static constexpr float FRAME_LEFT_MARGIN_REL = 0.10f;
	static constexpr float FRAME_RIGHT_MARGIN_REL = 0.10f;

	void DrawFrame();
	void DrawData();

	friend class Plot2DTest;
};

}  // namespace plotcpp

#endif  // _PLOTCPP_INCLUDE_PLOT2D_HPP_
