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

#include "Figure.hpp"

#include <fstream>
#include <string>
#include <thread>

#include "DisplayService.hpp"

namespace plotcpp {

void Figure::SetTitle(const std::string& title) { m_title = title; }

std::string Figure::Title() const { return m_title; }

void Figure::SetSize(unsigned int width, unsigned int height) {
  m_width = width;
  m_height = height;
}

unsigned int Figure::Width() const { return m_width; }

unsigned int Figure::Height() const { return m_height; }

std::string Figure::GetSVGText() const { return m_svg.GetText(); }

svg::Document& Figure::GetSVGDocument() { return m_svg; }

void Figure::Show() const {
  DisplayService& display_service = DisplayService::GetInstance();
  display_service.ShowFigure(this);
}

[[nodiscard]] std::thread Figure::ShowThread() const {
  return std::thread(&Figure::Show, this);
}

void Figure::Save(const std::string& filepath) const {
  // TODO: Extract extension and call save function
  // auto ext = filepath.find_last_of(".") + 1;
  // if (filepath.substr(ext) == "ext")

  // Default to svg
  SaveSVG(filepath);
}

void Figure::SaveSVG(const std::string& filepath) const {
  std::ofstream out_file(filepath);
  out_file << m_svg.GetText();
  out_file.close();
}

}  // namespace plotcpp
