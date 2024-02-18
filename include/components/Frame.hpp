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

#ifndef _PLOTCPP_INCLUDE_COMPONENTS_FRAME_HPP_
#define _PLOTCPP_INCLUDE_COMPONENTS_FRAME_HPP_

#include <set>
#include <string>

#include "components/text.hpp"
#include "style.hpp"
#include "svg.hpp"

namespace plotcpp {
namespace components {

class Frame {
public:
  enum class FrameType {
    FULL_FRAME,
    AXES_ONLY,
  };

  Frame(float width, float height, bool enable_grid);

  void Draw(svg::Document *document, float x, float y) const;

  void AddLeftMarker(float pos, const std::string &text);
  void AddTopMarker(float pos, const std::string &text);
  void AddRightMarker(float pos, const std::string &text);
  void AddBottomMarker(float pos, const std::string &text);

  void SetFrameType(FrameType frame_type);

protected:
  float m_width, m_height;
  float m_axis_font_size = 11.0f;
  bool m_grid_enable;
  FrameType m_frame_type = FrameType::FULL_FRAME;

  using Marker = std::pair<float, std::string>;
  std::set<Marker> m_left_markers;
  std::set<Marker> m_top_markers;
  std::set<Marker> m_right_markers;
  std::set<Marker> m_bottom_markers;

  const std::string RECT_CLIP_PATH_ID{"rect-clip-path"};
  static constexpr Color STROKE_COLOR = style::BORDER_COLOR;
  static constexpr float STROKE_WIDTH = 0.75f;
  const std::string DASH_ARRAY{"0.75,0.75"};
  static constexpr float MARKER_LENGTH = 5.0f;

  void DrawFullFrame(svg::Document *document, float x, float y) const;
  void DrawAxesOnly(svg::Document *document, float x, float y) const;
  void DrawAxes(svg::Document *document, float x, float y) const;
};

} // namespace components
} // namespace plotcpp

#endif // _PLOTCPP_INCLUDE_COMPONENTS_FRAME_HPP_
