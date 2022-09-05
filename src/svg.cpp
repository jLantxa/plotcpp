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

Document::~Document() {
	xmlFreeDoc(m_doc);
}

std::string Document::GetText() const {
	xmlChar* xml_str;
	xmlDocDumpFormatMemory(m_doc, &xml_str, nullptr, 1);
	return std::string{(char*) xml_str};
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

void Path::Add(const PathCommand& command) {
	commands.push_back(command);
}

void Path::Clear() {
	commands.clear();
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

void Document::DrawBackground(RGB color) {
	auto* node = AppendNode(m_root, "rect");

	SetAttribute(node, "x", "0");
	SetAttribute(node, "y", "0");
	SetAttribute(node, "width", "100", "%");
	SetAttribute(node, "height", "100", "%");

	std::stringstream style_value_ss;
	style_value_ss <<
		"stroke-opacity:" << "1.0" << "; " <<
		"fill: RGB(" <<
			std::to_string(color.r) << ", " <<
			std::to_string(color.g) << ", " <<
			std::to_string(color.b) << ");";
	SetAttribute(node, "style", style_value_ss.str());
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

void Document::DrawPath(const Path& path) {
	auto* node = AppendNode(m_root, "path");

	std::stringstream path_ss;
	for (const PathCommand& cmd : path.commands) {
		path_ss << cmd.ToString() << " ";
	}

	std::stringstream stroke_ss;
	stroke_ss << "RGB(" <<
		std::to_string(path.stroke_color.r) << ", " <<
		std::to_string(path.stroke_color.g) << ", " <<
		std::to_string(path.stroke_color.b) << ")";

	if (path.fill_transparent == true) {
		SetAttribute(node, "fill", "transparent");
	} else {
		std::stringstream ss;
		ss << "RGB(" <<
			std::to_string(path.fill_color.r) << ", " <<
			std::to_string(path.fill_color.g) << ", " <<
			std::to_string(path.fill_color.b) << ")";
		SetAttribute(node, "fill", ss.str());
	}

	SetAttribute(node, "stroke", stroke_ss.str());
	SetAttribute(node, "d", path_ss.str());
}

}  // namespace svg
}  // namespace plotcpp
