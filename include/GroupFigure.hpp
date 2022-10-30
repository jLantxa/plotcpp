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
 public:
  GroupFigure() = default;

  /**
   * \brief Add a subplot at the specified location in the group.
   * \param figure A pointer to a figure.
   * \param row Row
   * \param col Column
   */
  void Subplot(Figure* figure, std::size_t row, std::size_t col) {
    if ((row >= _rows) && (col >= _cols)) {
      return;
    }

    m_figures[_cols * row + col] = figure;
  }

  /**
   * \brief Returns a pointer to the figure at the specified location in the
   * group. \param row Row \param col Column
   */
  Figure* GetFigure(std::size_t row, std::size_t col) {
    return m_figures[_cols * row + col];
  }

  void Build() override {
    m_svg.DrawBackground({255, 255, 255});

    const unsigned int subplot_width = m_width / _cols;
    const unsigned int subplot_height = m_height / _rows;

    for (std::size_t i = 0; i < _rows; ++i) {
      for (std::size_t j = 0; j < _cols; ++j) {
        Figure* figure = GetFigure(i, j);
        if (figure == nullptr) {
          continue;
        }

        figure->SetSize(subplot_width, subplot_height);
        figure->Build();
        xmlDocPtr original_doc = figure->GetSVGDocument().GetDoc();
        xmlNodePtr original_root = xmlDocGetRootElement(original_doc);
        xmlNodePtr cloned_root = xmlCopyNode(original_root, 1);
        svg::SetAttribute(cloned_root, "x", std::to_string(j * subplot_width));
        svg::SetAttribute(cloned_root, "y", std::to_string(i * subplot_height));
        m_svg.Append(cloned_root);
      }
    }
  }

 protected:
  std::array<Figure*, _rows * _cols> m_figures;

};  // namespace plotcpp

}  // namespace plotcpp

#endif  // _PLOTCPP_INCLUDE_GROUPFIGURE_HPP_
