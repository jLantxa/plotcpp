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

#include "DisplayService.hpp"

#include <GLFW/glfw3.h>
#include <librsvg/rsvg.h>

#include "Figure.hpp"

namespace plotcpp {

DisplayService::DisplayService() noexcept {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

const DisplayService& DisplayService::GetInstance() {
  static DisplayService instance;
  return instance;
}

void DisplayService::ShowFigure(const Figure* figure) const {
  FigureWindow window(figure);
  window.Show();
}

cairo_surface_t* DisplayService::RenderToSurface(const Figure* figure) const {
  const int width = static_cast<int>(figure->Width());
  const int height = static_cast<int>(figure->Height());

  const std::string svg_str = figure->GetSVGText();
  const uint8_t* svg_data = (const uint8_t*)(svg_str.c_str());
  RsvgHandle* handle =
      rsvg_handle_new_from_data(svg_data, svg_str.size(), &error);

  cairo_surface_t* surface =
      cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  cairo_t* cr = cairo_create(surface);

  RsvgRectangle viewport = {
      .x = 0.0,
      .y = 0.0,
      .width = static_cast<float>(width),
      .height = static_cast<float>(height),
  };

  rsvg_handle_render_document(handle, cr, &viewport, &error);

  cairo_destroy(cr);

  return surface;
}

FigureWindow::FigureWindow(const Figure* figure) : m_figure(figure) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void FigureWindow::Show() {
  if (m_figure == nullptr) {
    return;
  }

  const int width = static_cast<int>(m_figure->Width());
  const int height = static_cast<int>(m_figure->Height());
  GLFWwindow* window =
      glfwCreateWindow(width, height, m_figure->Title().c_str(), NULL, NULL);
  glfwSetWindowAttrib(window, GLFW_RESIZABLE, false);
  glfwMakeContextCurrent(window);

  // Render SVG to cairo surface
  const DisplayService& display_service = DisplayService::GetInstance();
  cairo_surface_t* surface = display_service.RenderToSurface(m_figure);
  // TODO: Draw surface

  // Poll events
  m_is_running = true;
  while (m_is_running && !glfwWindowShouldClose(window)) {
    glfwPollEvents();
    usleep(33'000);
  }

  // Clean up
  cairo_surface_destroy(surface);
  glfwDestroyWindow(window);
}

void FigureWindow::Close() { m_is_running = false; }

}  // namespace plotcpp
