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

#include <deque>
#include <functional>
#include <map>
#include <string>

#include "BarPlotHandler.hpp"
#include "HistogramHandler.hpp"
#include "Plot2DHandler.hpp"

static bool HandlePlot2D(int argc, char** argv) {
  Plot2DHandler handler;
  return handler.Run(argc, argv);
}

static bool HandleBarPlot(int argc, char** argv) {
  BarPlotHandler handler;
  return handler.Run(argc, argv);
}

static bool HandleHistogramPlot(int argc, char** argv) {
  HistogramHandler handler;
  return handler.Run(argc, argv);
}

static const std::map<std::string, std::function<bool(int, char**)>> PROGRAMS{
    {"plot2d", HandlePlot2D},
    {"bar", HandleBarPlot},
    {"hist", HandleHistogramPlot},
};

static bool HandleArguments(int argc, char** argv) {
  if (argc < 1) {
    return false;
  }

  const auto it = PROGRAMS.find(argv[0]);
  if (it != PROGRAMS.end()) {
    const auto program_handler = it->second;
    return program_handler(argc, argv);
  }

  return false;
}

int main(int argc, char** argv) {
  if (!HandleArguments(argc - 1, argv + 1)) {
    return -1;
  }

  return 0;
}
