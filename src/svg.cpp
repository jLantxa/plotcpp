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

#include "svg.hpp"

#include "libxml/tree.h"

namespace plotcpp {
namespace svg {

static inline const xmlChar* xchar(const char* str) {
  return (const xmlChar*)str;
}

xmlNode* AppendNode(xmlNode* parent, const std::string& name) {
  xmlNode* new_node = xmlNewNode(nullptr, xchar(name.c_str()));
  xmlAddChild(parent, new_node);
  return new_node;
}

void SetAttribute(xmlNode* node, const std::string& name,
                  const std::string& value, const std::string& unit) {
  const std::string val_str = value + unit;
  xmlSetProp(node, xchar(name.c_str()), xchar(val_str.c_str()));
}

Document::Document() {
  m_doc = xmlNewDoc(xchar("1.0"));
  Reset();
}

Document::~Document() { xmlFreeDoc(m_doc); }

xmlDocPtr Document::GetDoc() { return m_doc; }

std::string Document::GetText() const {
  xmlChar* xml_str;
  xmlDocDumpFormatMemory(m_doc, &xml_str, nullptr, 1);
  return std::string{(char*)xml_str};
}

static std::string ColorToString(const Color& color) {
  std::stringstream ss;
  ss << "RGB(" << std::to_string(color.r) << ", " << std::to_string(color.g)
     << ", " << std::to_string(color.b) << ")";
  return ss.str();
}

std::string PathCommand::ToString() const {
  std::stringstream ss;

  switch (id) {
    case Id::MOVE:
      ss << "M";
      break;
    case Id::MOVE_R:
      ss << "m";
      break;
    case Id::LINE:
      ss << "L";
      break;
    case Id::LINE_R:
      ss << "l";
      break;
    case Id::CLOSE:
      ss << "Z";
  }

  /* The close (Z) command has no arguments */
  if (id != Id::CLOSE) {
    ss << " " << std::to_string(x) << " " << std::to_string(y);
  }

  return ss.str();
}

void Path::Add(const PathCommand& command) { commands.push_back(command); }

void Path::Clear() { commands.clear(); }

void Document::Reset() {
  if (m_root != nullptr) {
    xmlUnlinkNode(m_root);
    xmlFreeNode(m_root);
  }

  m_root = xmlNewNode(nullptr, xchar("svg"));
  xmlDocSetRootElement(m_doc, m_root);

  m_defs = AppendNode(m_root, "defs");
}

void Document::SetSize(unsigned int width, unsigned int height) {
  m_width = width;
  m_height = height;
  SetAttribute(m_root, "width", std::to_string(m_width));
  SetAttribute(m_root, "height", std::to_string(m_height));
}

void Document::Append(xmlNodePtr node) { xmlAddChild(m_root, node); }

xmlNodePtr Document::AddGroup(xmlNodePtr parent_node, const std::string& id) {
  xmlNodePtr parent = parent_node == nullptr ? m_root : parent_node;
  auto* node = AppendNode(parent, "g");

  if (!id.empty()) {
    SetAttribute(node, "id", id);
  }

  return node;
}

xmlNodePtr Document::Defs() { return m_defs; }

xmlNodePtr Document::DrawBackground(Color color) {
  auto* node = AppendNode(m_root, "rect");

  SetAttribute(node, "id", "_background");

  SetAttribute(node, "x", "0");
  SetAttribute(node, "y", "0");
  SetAttribute(node, "width", "100", "%");
  SetAttribute(node, "height", "100", "%");

  std::stringstream style_value_ss;
  style_value_ss << "stroke-opacity:"
                 << "1.0"
                 << "; "
                 << "fill: " << ColorToString(color) << ";";
  SetAttribute(node, "style", style_value_ss.str());

  return node;
}

xmlNodePtr Document::DrawLine(const Line& line, xmlNodePtr parent_node,
                              const std::string& id) {
  xmlNodePtr parent = parent_node == nullptr ? m_root : parent_node;
  auto* node = AppendNode(parent, "line");

  if (!id.empty()) {
    SetAttribute(node, "id", id);
  }

  SetAttribute(node, "x1", std::to_string(line.x1));
  SetAttribute(node, "y1", std::to_string(line.y1));
  SetAttribute(node, "x2", std::to_string(line.x2));
  SetAttribute(node, "y2", std::to_string(line.y2));
  SetAttribute(node, "stroke", ColorToString(line.stroke_color));
  SetAttribute(node, "stroke-width", std::to_string(line.stroke_width));
  SetAttribute(node, "stroke-opacity", std::to_string(line.stroke_opacity));

  return node;
}

xmlNodePtr Document::DrawRect(const Rect& rect, xmlNodePtr parent_node,
                              const std::string& id) {
  xmlNodePtr parent = parent_node == nullptr ? m_root : parent_node;
  auto* node = AppendNode(parent, "rect");

  if (!id.empty()) {
    SetAttribute(node, "id", id);
  }

  SetAttribute(node, "x", std::to_string(rect.x));
  SetAttribute(node, "y", std::to_string(rect.y));
  SetAttribute(node, "width", std::to_string(rect.width));
  SetAttribute(node, "height", std::to_string(rect.height));
  SetAttribute(node, "rx", std::to_string(rect.rx));
  SetAttribute(node, "ry", std::to_string(rect.ry));
  SetAttribute(node, "stroke", ColorToString(rect.stroke_color));
  SetAttribute(node, "stroke-width", std::to_string(rect.stroke_width));
  SetAttribute(node, "stroke-opacity", std::to_string(rect.stroke_opacity));

  if (rect.fill_transparent == true) {
    SetAttribute(node, "fill", "transparent");
  } else {
    SetAttribute(node, "fill", ColorToString(rect.fill_color));
    SetAttribute(node, "fill-opacity", std::to_string(rect.fill_opacity));
  }

  return node;
}

xmlNodePtr Document::DrawCircle(const Circle& circle, xmlNodePtr parent_node,
                                const std::string& id) {
  xmlNodePtr parent = parent_node == nullptr ? m_root : parent_node;
  auto* node = AppendNode(parent, "circle");

  if (!id.empty()) {
    SetAttribute(node, "id", id);
  }

  SetAttribute(node, "cx", std::to_string(circle.cx));
  SetAttribute(node, "cy", std::to_string(circle.cy));
  SetAttribute(node, "r", std::to_string(circle.r));
  SetAttribute(node, "fill", ColorToString(circle.fill_color));

  return node;
}

xmlNodePtr Document::DrawPath(const Path& path, xmlNodePtr parent_node,
                              const std::string& id) {
  xmlNodePtr parent = parent_node == nullptr ? m_root : parent_node;
  auto* node = AppendNode(parent, "path");

  if (!id.empty()) {
    SetAttribute(node, "id", id);
  }

  std::stringstream path_ss;
  for (const PathCommand& cmd : path.commands) {
    path_ss << cmd.ToString() << " ";
  }

  if (path.fill_transparent == true) {
    SetAttribute(node, "fill", "transparent");
  } else {
    SetAttribute(node, "fill", ColorToString(path.fill_color));
    SetAttribute(node, "fill-opacity", std::to_string(path.fill_opacity));
  }

  SetAttribute(node, "stroke", ColorToString(path.stroke_color));
  SetAttribute(node, "stroke-width", std::to_string(path.stroke_width));
  SetAttribute(node, "d", path_ss.str());

  return node;
}

xmlNodePtr Document::DrawText(const Text& text, xmlNodePtr parent_node,
                              const std::string& id) {
  xmlNodePtr parent = parent_node == nullptr ? m_root : parent_node;
  auto* node = AppendNode(parent, "text");

  if (!id.empty()) {
    SetAttribute(node, "id", id);
  }

  xmlNodeSetContent(node, xchar(text.text.c_str()));
  SetAttribute(node, "x", std::to_string(text.x));
  SetAttribute(node, "y", std::to_string(text.y));
  SetAttribute(node, "font-size", std::to_string(text.font_size));

  if (!text.font_family.empty()) {
    SetAttribute(node, "font-family", text.font_family);
  }

  return node;
}

}  // namespace svg
}  // namespace plotcpp
