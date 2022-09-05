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

#include <algorithm>
#include <limits>
#include <numeric>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "Plot2D.hpp"
#include "svg.hpp"

namespace plotcpp {

static constexpr std::array<svg::RGB, 4> PLOT_COLORS {
    svg::RGB{32, 112, 255},
    {255, 32, 32},
    {16, 192, 16},
    {0, 0, 0}
};

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
    m_x_set_range = {x_min, x_max};
}

void Plot2D::SetYRange(Real y0, Real y1) {
    const Real y_min = std::min(y0, y1);
    const Real y_max = std::max(y0, y1);
    m_y_set_range = {y_min, y_max};
}

std::optional<Plot2D::Range> Plot2D::GetXRange() const {
    return m_x_set_range;
}

std::optional<Plot2D::Range> Plot2D::GetYRange() const {
    return m_y_set_range;
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
    m_x_set_range.reset();
    m_y_set_range.reset();
    m_svg.Reset();
}

void Plot2D::ClearData() {
    m_data.clear();
}

void Plot2D::Build() {
    m_svg.Reset();
    m_svg.SetSize(m_width, m_height);

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

    CalculateFrame();

    DrawBackground();
    DrawData();
    DrawFrame();
}

void Plot2D::CalculateFrame() {
    // Frame rectangle
    m_frame_x = m_width * FRAME_LEFT_MARGIN_REL;
    m_frame_y = m_height * (FRAME_TOP_MARGIN_REL);
    m_frame_w = m_width * (1.0f - FRAME_LEFT_MARGIN_REL - FRAME_RIGHT_MARGIN_REL);
    m_frame_h = m_height * (1.0f - FRAME_TOP_MARGIN_REL - FRAME_BOTTOM_MARGIN_REL);

    // Ranges
    Real min_x = std::numeric_limits<Real>::max();
    Real max_x = std::numeric_limits<Real>::min();
    Real min_y = std::numeric_limits<Real>::max();
    Real max_y = std::numeric_limits<Real>::min();
    for (auto& plot : m_data) {
        const std::size_t size = plot.first.size();
        for (std::size_t i = 0; i < size; ++i) {
            const Real x = plot.first[i];
            const Real y = plot.second[i];

            min_x = std::min(x, min_x);
            max_x = std::max(x, max_x);
            min_y = std::min(y, min_y);
            max_y = std::max(y, max_y);
        }
    }
    m_x_data_range = {min_x, max_x};
    m_y_data_range = {min_y, max_y};

    m_x_range = m_x_set_range.has_value()? m_x_set_range.value() : m_x_data_range;
    m_y_range = m_y_set_range.has_value()? m_y_set_range.value() : m_y_data_range;

    // Zoom factors
    m_zoom_x = abs(m_frame_w / (m_x_range.second - m_x_range.first));
    m_zoom_y = abs(m_frame_h / (m_y_range.second - m_y_range.first));
}

std::pair<float, float> Plot2D::TranslateToFrame(Real x, Real y) const {
    float tx = (m_zoom_x * (x + m_x_range.first)) + m_frame_x;
    float ty = -(m_zoom_y * (y - m_y_range.first)) + m_frame_y + m_frame_h;
    return {tx, ty};
}

void Plot2D::DrawBackground() {
    m_svg.DrawBackground(svg::RGB{255, 255, 255});
}

void Plot2D::DrawFrame() {
    svg::Rect frame_rect {
        .x = m_frame_x,
        .y = m_frame_y,
        .width = m_frame_w,
        .height = m_frame_h,
        .fill_opacity = 0.0f,
    };
    m_svg.DrawRect(frame_rect);

    // TODO: Add clip-path rectangle
}

void Plot2D::DrawData() {
    const std::size_t num_plots = m_data.size();
    for (std::size_t p = 0; p < num_plots; ++p) {
        const DataPair& plot = m_data[p];

        svg::Path path;
        path.stroke_color = PLOT_COLORS[p % PLOT_COLORS.size()];
        path.stroke_width = 2;

        const std::vector<Real>& data_x = plot.first;
        const std::vector<Real>& data_y = plot.second;
        const std::size_t size = data_x.size();

        for (std::size_t i = 0; i < size; ++i) {
            const auto path_cmd = (i > 0)? svg::PathCommand::Id::LINE : svg::PathCommand::Id::MOVE;
            const auto trans_point = TranslateToFrame(data_x[i], data_y[i]);
            path.Add({path_cmd, trans_point.first, trans_point.second});
       }

        m_svg.DrawPath(path);
    }
}

}  // namespace plotcpp
