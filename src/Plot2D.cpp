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
#include <numeric>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "Plot2D.hpp"

namespace plotcpp {

Plot2D::Plot2D() : Figure() {}

void Plot2D::Plot(const std::vector<Real>& x_data, const std::vector<Real>& y_data) {
    const std::size_t size = x_data.size();

    if (x_data.size() != y_data.size()) {
        return;
    }

    DataPair data_pair;
    data_pair.first.resize(size);
    data_pair.second.resize(size);
    std::copy(x_data.begin(), x_data.end(), data_pair.first.begin());
    std::copy(y_data.begin(), y_data.end(), data_pair.second.begin());
    m_data.emplace_back(data_pair);
}

void Plot2D::Plot(const std::vector<Real>& y_data) {
    std::vector<Real> x_data;
    x_data.resize(y_data.size());

    std::iota(x_data.begin(), x_data.end(), 1.0f);
    Plot(x_data, y_data);
}

void Plot2D::SetXRange(Real x0, Real x1) {
    const Real x_min = std::min(x0, x1);
    const Real x_max = std::max(x0, x1);
    m_x_range = {x_min, x_max};
}

void Plot2D::SetYRange(Real y0, Real y1) {
    const Real y_min = std::min(y0, y1);
    const Real y_max = std::max(y0, y1);
    m_y_range = {y_min, y_max};
}

std::optional<std::pair<Real, Real>> Plot2D::GetXRange() const {
    return m_x_range;
}

std::optional<std::pair<Real, Real>> Plot2D::GetYRange() const {
    return m_y_range;
}

void Plot2D::SetXLabel(const std::string& label) {
    m_x_label = label;
}

void Plot2D::SetYLabel(const std::string& label) {
    m_y_label = label;
}

std::string Plot2D::GetXLabel() const {
    return m_x_label;
}

std::string Plot2D::GetYLabel() const {
    return m_y_label;
}

void Plot2D::Clear() {
    ClearData();
    SetTitle("");
    SetSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    SetXLabel("");
    SetYLabel("");
    m_x_range.reset();
    m_y_range.reset();
}

void Plot2D::ClearData() {
    m_data.clear();
}

void Plot2D::Build() {
    /* TODO(jLantxa):
        * Generate graphic primitives from bottom to top:
        * 1. Add title (if defined)
        * 2. Add labels (if defined). [x label must be translated and rotated]
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

}  // namespace plotcpp
