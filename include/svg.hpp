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

#include <cstdint>

#include <sstream>
#include <string>
#include <vector>

#include <libxml/parser.h>
#include <libxml/tree.h>

namespace plotcpp {

namespace svg {

struct RGB {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

struct Line {
	float x1, y1, x2, y2;
	RGB stroke_color {0, 0, 0};
	float stroke_opacity = 1.0f;
	float stroke_width = 1;
};

struct Rect {
	float x, y;
	float width, height;
	float rx = 0;
	float ry = 0;
	RGB stroke_color {0, 0, 0};
	float stroke_opacity = 1.0f;
	float stroke_width = 1;
	RGB fill_color {255, 255, 255};
	float fill_opacity = 1.0f;
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
	RGB stroke_color {0, 0, 0};
	RGB fill_color {0, 0, 0};
	bool fill_transparent = true;

	void Add(const PathCommand& command);
	void Clear();
};

/**
 * /brief An SVG document
 */
class Document {
public:
	Document();

	/** Return the xml text */
	std::string GetText() const;

	/** Clear all elements in this document */
	void Clear();

	/**
	 * \brief Set the Size object
	 *
	 * \param width Image width in px
	 * \param height Image height in px
	 */
	void SetSize(unsigned int width, unsigned int height);

	/** Draw a line */
	void DrawLine(const Line& line);

	/** Draw a rectangle */
	void DrawRect(const Rect& rect);

	/** Draw a path */
	void DrawPath(const Path& path);

private:
	unsigned int m_width, m_height;
	xmlDocPtr m_doc;
	xmlNodePtr m_root;

	xmlNode* AppendNode(xmlNode* parent, const std::string& name);

	void SetAttribute(xmlNode* node,
					const std::string& name,
					const std::string& value,
					const std::string& unit = "");

};

}  // namespace svg

}  // namespace plotcpp

#endif  // _PLOTCPP_INCLUDE_SVG_HPP_