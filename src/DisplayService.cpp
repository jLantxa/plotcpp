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

#include "Figure.hpp"

namespace plotcpp {

DisplayService::DisplayService() noexcept {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

DisplayService& DisplayService::GetInstance() {
  static DisplayService instance;
  return instance;
}

void DisplayService::ShowFigure(const Figure* figure) {
  FigureWindow window(figure);
  window.Show();
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

  m_is_running = true;
  while (m_is_running && !glfwWindowShouldClose(window)) {
    glfwPollEvents();
    usleep(33'000);
  }

  glfwDestroyWindow(window);
}

void FigureWindow::Close() { m_is_running = false; }

}  // namespace plotcpp
