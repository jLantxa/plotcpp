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

namespace plotcpp {
namespace svg {

static inline const xmlChar* xchar(const char* str) {
	return (const xmlChar*) str;
}

xmlNode* Document::AppendNode(xmlNode* parent, const std::string& name) {
	xmlNode* new_node = xmlNewNode(nullptr, xchar(name.c_str()));
	xmlAddChild(parent, new_node);
	return new_node;
}

void Document::SetAttribute(xmlNode* node,
				  const std::string& name,
				  const std::string& value,
				  const std::string& unit)
{
	const std::string val_str = value + unit;
	xmlSetProp(node, xchar(name.c_str()), xchar(val_str.c_str()));
}

Document::Document() {
	m_doc = xmlNewDoc(xchar("1.0"));
	m_root = xmlNewNode(nullptr, xchar("svg"));
	xmlDocSetRootElement(m_doc, m_root);
}

std::string Document::GetText() const {
	xmlChar* xml_str;
	xmlDocDumpFormatMemory(m_doc, &xml_str, nullptr, 1);
	return std::string{(char*) xml_str};
}

void Document::Clear() {
	// TODO
}

void Document::SetSize(unsigned int width, unsigned int height) {
	m_width = width;
	m_height = height;
	SetAttribute(m_root, "width", std::to_string(m_width));
	SetAttribute(m_root, "height", std::to_string(m_height));
}

void Document::DrawLine(const Line& line) {
	auto* node = AppendNode(m_root, "line");

	SetAttribute(node, "x1", std::to_string(line.x1));
	SetAttribute(node, "y1", std::to_string(line.y1));
	SetAttribute(node, "x2", std::to_string(line.x2));
	SetAttribute(node, "y2", std::to_string(line.y2));

	std::stringstream style_value_ss;
	style_value_ss <<
		"stroke: RGB(" <<
			std::to_string(line.stroke_color.r) << ", " <<
			std::to_string(line.stroke_color.g) << ", " <<
			std::to_string(line.stroke_color.b) << "); " <<
		"stroke-width:" << std::to_string(line.stroke_width) << "; " <<
		"stroke-opacity:" << std::to_string(line.stroke_opacity);
	SetAttribute(node, "style", style_value_ss.str());
}

void Document::DrawRect(const Rect& rect) {
	auto* node = AppendNode(m_root, "rect");

	SetAttribute(node, "x", std::to_string(rect.x));
	SetAttribute(node, "y", std::to_string(rect.y));
	SetAttribute(node, "width", std::to_string(rect.width));
	SetAttribute(node, "height", std::to_string(rect.height));
	SetAttribute(node, "rx", std::to_string(rect.rx));
	SetAttribute(node, "ry", std::to_string(rect.ry));

	std::stringstream style_value_ss;
	style_value_ss <<
		"stroke: RGB(" <<
			std::to_string(rect.stroke_color.r) << ", " <<
			std::to_string(rect.stroke_color.g) << ", " <<
			std::to_string(rect.stroke_color.b) << "); " <<
		"stroke-width:" << std::to_string(rect.stroke_width) << "; " <<
		"stroke-opacity:" << std::to_string(rect.stroke_opacity) << "; " <<
		"fill: RGB(" <<
			std::to_string(rect.fill_color.r) << ", " <<
			std::to_string(rect.fill_color.g) << ", " <<
			std::to_string(rect.fill_color.b) << "); " <<
		"fill-opacity:" << std::to_string(rect.fill_opacity);
	SetAttribute(node, "style", style_value_ss.str());
}

}  // namespace svg
}  // namespace plotcpp
