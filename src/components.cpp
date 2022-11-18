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

#include "components.hpp"

#include <fmt/format.h>

#include <set>
#include <string>

#include "fonts.hpp"
#include "svg.hpp"

namespace plotcpp {
namespace components {

Frame::Frame(float width, float height, bool enable_grid)
    : m_width(width), m_height(height), m_grid_enable(enable_grid) {}

void Frame::AddLeftMarker(float pos, const std::string& text) {
  m_left_markers.insert({pos, text});
}

void Frame::AddTopMarker(float pos, const std::string& text) {
  m_top_markers.insert({pos, text});
}

void Frame::AddRightMarker(float pos, const std::string& text) {
  m_right_markers.insert({pos, text});
}

void Frame::AddBottomMarker(float pos, const std::string& text) {
  m_bottom_markers.insert({pos, text});
}

void Frame::SetFrameType(FrameType frame_type) { m_frame_type = frame_type; }

void Frame::Draw(svg::Document* document, float x, float y) const {
  DrawAxes(document, x, y);

  // Draw frame
  switch (m_frame_type) {
    case FrameType::FULL_FRAME:
      DrawFullFrame(document, x, y);
      break;
    case FrameType::AXES_ONLY:
      DrawAxesOnly(document, x, y);
      break;
  }
}

void Frame::DrawFullFrame(svg::Document* document, float x, float y) const {
  svg::Rect frame_rect{
      .x = x,
      .y = y,
      .width = m_width,
      .height = m_height,
      .stroke_color = STROKE_COLOR,
  };
  auto frame_node = document->DrawRect(frame_rect);
  svg::SetAttribute(frame_node, "fill", "none");

  auto defs_node = document->Defs();
  auto clip_path_node = svg::AppendNode(defs_node, "clipPath");
  svg::SetAttribute(clip_path_node, "id", RECT_CLIP_PATH_ID);
  svg::Rect clip_rect{.x = x, .y = y, .width = m_width, .height = m_height};
  document->DrawRect(clip_rect, clip_path_node);
}

void Frame::DrawAxesOnly(svg::Document* document, float x, float y) const {
  svg::Rect frame_rect{
      .x = x,
      .y = y,
      .width = m_width,
      .height = m_height,
      .stroke_color = STROKE_COLOR,
      .stroke_opacity = 0.0f,
      .stroke_width = 1,
      .fill_color{255, 255, 255},
      .fill_opacity = 0.0f,
      .fill_transparent = true,
  };
  auto frame_node = document->DrawRect(frame_rect);
  svg::SetAttribute(frame_node, "fill", "none");

  document->DrawLine({x, y, x, y + m_height, STROKE_COLOR});
  document->DrawLine(
      {x, y + m_height, x + m_width, y + m_height, STROKE_COLOR});

  auto defs_node = document->Defs();
  auto clip_path_node = svg::AppendNode(defs_node, "clipPath");
  svg::SetAttribute(clip_path_node, "id", RECT_CLIP_PATH_ID);
  svg::Rect clip_rect{.x = x, .y = y, .width = m_width, .height = m_height};
  document->DrawRect(clip_rect, clip_path_node);
}

void Frame::DrawAxes(svg::Document* document, float x, float y) const {
  float font_em = m_axis_font_size / 12.0f;

  // Left markers
  for (const Marker& marker : m_left_markers) {
    svg::Line marker_line{.x1 = x,
                          .y1 = y + marker.first,
                          .x2 = x - MARKER_LENGTH,
                          .y2 = y + marker.first,
                          .stroke_color = STROKE_COLOR,
                          .stroke_opacity = 1.0f,
                          .stroke_width = 1};
    document->DrawLine(marker_line);

    // TODO: Draw text
    auto text_node = document->DrawText(
        svg::Text{marker.second, x - 2 * MARKER_LENGTH,
                  y + marker.first + fonts::EmToPx(font_em / 4.0f),
                  m_axis_font_size, components::TEXT_FONT});
    svg::SetAttribute(text_node, "text-anchor", "end");

    if (m_grid_enable) {
      svg::Line grid_line{.x1 = x,
                          .y1 = y + marker.first,
                          .x2 = x + m_width,
                          .y2 = y + marker.first,
                          .stroke_color = STROKE_COLOR,
                          .stroke_opacity = 1.0f,
                          .stroke_width = 0.75f};
      auto grid_line_node = document->DrawLine(grid_line);
      svg::SetAttribute(grid_line_node, "stroke-dasharray", DASH_ARRAY);
    }
  }

  // TODO: Top markers

  // TODO: Right markers

  // Bottom markers
  for (const Marker& marker : m_bottom_markers) {
    svg::Line marker_line{.x1 = x + marker.first,
                          .y1 = y + m_height,
                          .x2 = x + marker.first,
                          .y2 = y + m_height + MARKER_LENGTH,
                          .stroke_color = STROKE_COLOR,
                          .stroke_opacity = 1.0f,
                          .stroke_width = 1};
    document->DrawLine(marker_line);

    auto text_node = document->DrawText(
        svg::Text{marker.second, x + marker.first,
                  y + m_height + MARKER_LENGTH + fonts::EmToPx(font_em),
                  m_axis_font_size, components::TEXT_FONT});
    svg::SetAttribute(text_node, "text-anchor", "middle");

    if (m_grid_enable) {
      svg::Line grid_line{.x1 = x + marker.first,
                          .y1 = y,
                          .x2 = x + marker.first,
                          .y2 = y + m_height,
                          .stroke_color = STROKE_COLOR,
                          .stroke_opacity = 1.0f,
                          .stroke_width = 0.75f};
      auto grid_line_node = document->DrawLine(grid_line);
      svg::SetAttribute(grid_line_node, "stroke-dasharray", DASH_ARRAY);
    }
  }
}

}  // namespace components
}  // namespace plotcpp
