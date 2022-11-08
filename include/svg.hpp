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

#ifndef _PLOTCPP_INCLUDE_SVG_HPP_
#define _PLOTCPP_INCLUDE_SVG_HPP_

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

#include "utility.hpp"

namespace plotcpp {

namespace svg {

xmlNode* AppendNode(xmlNode* parent, const std::string& name);

void SetAttribute(xmlNode* node, const std::string& name,
                  const std::string& value, const std::string& unit = "");

struct Line {
  float x1, y1, x2, y2;
  Color stroke_color{0, 0, 0};
  float stroke_opacity = 1.0f;
  float stroke_width = 1;
};

struct Rect {
  float x, y;
  float width, height;
  float rx = 0;
  float ry = 0;
  Color stroke_color{0, 0, 0};
  float stroke_opacity = 1.0f;
  float stroke_width = 1;
  Color fill_color{255, 255, 255};
  float fill_opacity = 1.0f;
  bool fill_transparent = true;
};

struct Circle {
  float cx, cy;
  float r;
  Color fill_color{0, 0, 0};
};

struct PathCommand {
  enum class Id {
    MOVE,
    MOVE_R,
    LINE,
    LINE_R,
    CLOSE,
  };

  Id id;
  float x, y;

  std::string ToString() const;
};

struct Path {
  std::vector<PathCommand> commands;
  float stroke_width = 1;
  Color stroke_color{0, 0, 0};
  float stroke_opacity = 1.0f;
  Color fill_color{0, 0, 0};
  float fill_opacity = 1.0f;
  bool fill_transparent = true;

  void Add(const PathCommand& command);
  void Clear();
};

struct Text {
  std::string text;
  float x, y;
  float font_size = 12;
  std::string font_family;
  Color color = {0, 0, 0};
};

/**
 * /brief An SVG document
 */
class Document {
 public:
  Document();

  ~Document();

  /** Return the xml text */
  std::string GetText() const;

  /** Clear all elements in this document */
  void Reset();

  xmlDocPtr GetDoc();

  /**
   * \brief Set the Size object
   *
   * \param width Image width in px
   * \param height Image height in px
   */
  void SetSize(unsigned int width, unsigned int height);

  void Append(xmlNodePtr node);

  /** Draw background color */
  xmlNodePtr DrawBackground(Color color);

  /** Draw a line */
  xmlNodePtr DrawLine(const Line& line, xmlNodePtr parent_node = nullptr,
                      const std::string& id = "");

  /** Draw a rectangle */
  xmlNodePtr DrawRect(const Rect& rect, xmlNodePtr parent_node = nullptr,
                      const std::string& id = "");

  /** Draw a circle */
  xmlNodePtr DrawCircle(const Circle& circle, xmlNodePtr parent_node = nullptr,
                        const std::string& id = "");

  /** Draw a path */
  xmlNodePtr DrawPath(const Path& path, xmlNodePtr parent_node = nullptr,
                      const std::string& id = "");

  /** Draw text */
  xmlNodePtr DrawText(const Text& text, xmlNodePtr parent_node = nullptr,
                      const std::string& id = "");

  /** Add a group node */
  xmlNodePtr AddGroup(xmlNodePtr parent_node = nullptr,
                      const std::string& id = "");

  /** Get the <defs> node */
  xmlNodePtr Defs();

 private:
  unsigned int m_width, m_height;
  xmlDocPtr m_doc = nullptr;
  xmlNodePtr m_root = nullptr;
  xmlNodePtr m_defs = nullptr;
};

}  // namespace svg

}  // namespace plotcpp

#endif  // _PLOTCPP_INCLUDE_SVG_HPP_
