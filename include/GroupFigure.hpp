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

#ifndef _PLOTCPP_INCLUDE_GROUPFIGURE_HPP_
#define _PLOTCPP_INCLUDE_GROUPFIGURE_HPP_

#include <array>
#include <string>

#include "Figure.hpp"
#include "libxml/tree.h"
#include "svg.hpp"

namespace plotcpp {

template <std::size_t _rows, std::size_t _cols>
class GroupFigure : public Figure {
  static_assert(_rows > 0);
  static_assert(_cols > 0);

public:
  GroupFigure() { ClearFigures(); };

  /**
   * @brief Add a subplot at the specified location in the group.
   *
   * @param figure A pointer to a figure.
   * @param row Row
   * @param col Column
   */
  void Subplot(Figure *figure, std::size_t row, std::size_t col) {
    if ((row >= _rows) && (col >= _cols)) {
      return;
    }

    m_figures[_cols * row + col] = figure;
  }

  /**
   * @brief Returns a pointer to the figure at the specified location in the
   * group.
   *
   * @param row Row
   * @param col Column
   */
  Figure *GetFigure(std::size_t row, std::size_t col) {
    return m_figures[_cols * row + col];
  }

  void Clear() override { ClearFigures(); }

  void Build() override {
    m_svg.DrawBackground({255, 255, 255});

    const unsigned int subplot_width = m_width / _cols;
    const unsigned int subplot_height = m_height / _rows;

    for (unsigned int i = 0; i < _rows; ++i) {
      for (unsigned int j = 0; j < _cols; ++j) {
        Figure *figure = GetFigure(i, j);
        if (figure == nullptr) {
          continue;
        }

        const unsigned int x = j * subplot_width;
        const unsigned int y = i * subplot_height;

        figure->SetSize(subplot_width, subplot_height);
        figure->Build();
        xmlDocPtr original_doc = figure->GetSVGDocument().GetDoc();
        xmlNodePtr original_root = xmlDocGetRootElement(original_doc);
        xmlNodePtr cloned_root = xmlCopyNode(original_root, 1);
        svg::SetAttribute(cloned_root, "x", std::to_string(x));
        svg::SetAttribute(cloned_root, "y", std::to_string(y));
        m_svg.Append(cloned_root);
      }
    }
  }

protected:
  std::array<Figure *, _rows * _cols> m_figures;

  static constexpr unsigned int HORIZONTAL_MARGIN = 20;
  static constexpr unsigned int VERTICAL_MARGIN = 20;

  void ClearFigures() {
    for (auto &figure_ptr : m_figures) {
      figure_ptr = nullptr;
    }
  }

}; // namespace plotcpp

} // namespace plotcpp

#endif // _PLOTCPP_INCLUDE_GROUPFIGURE_HPP_
