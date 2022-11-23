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

#include "Plot2DHandler.hpp"

static bool HandlePlot2D(std::deque<std::string> args) {
  Plot2DHandler handler;
  return handler.Run(args);
}

static const std::map<std::string, std::function<bool(std::deque<std::string>)>>
    PROGRAMS{
        {"plot2d", HandlePlot2D},
    };

static std::deque<std::string> PackArgs(int argc, char* argv[]) {
  std::deque<std::string> args;

  for (int i = 0; i < argc; ++i) {
    args.push_back(argv[i]);
  }

  return args;
}

static bool HandleArguments(std::deque<std::string> args) {
  if (args.size() < 1) {
    return false;
  }

  const auto it = PROGRAMS.find(args[0]);
  if (it != PROGRAMS.end()) {
    args.pop_front();
    const auto program_handler = it->second;
    return program_handler(args);
  }

  return false;
}

int main(int argc, char* argv[]) {
  auto args = PackArgs(argc, argv);
  args.pop_front();

  if (!HandleArguments(args)) {
    return -1;
  }

  return 0;
}
