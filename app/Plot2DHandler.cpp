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

#include "Plot2DHandler.hpp"

#include <getopt.h>

#include <iostream>

#include "Plot2D.hpp"
#include "app.hpp"
#include "csv.hpp"

bool Plot2DHandler::Run(int argc, char** argv) {
  static constexpr int FIRST_LINE_LABEL_OPTION = 1000;

  static const struct option long_options[] = {
      {"filename", required_argument, nullptr, 'f'},
      {"first-line-labels", no_argument, nullptr, FIRST_LINE_LABEL_OPTION},
      {"title", required_argument, nullptr, 't'},
      {"x-label", required_argument, nullptr, 'x'},
      {"y-label", required_argument, nullptr, 'y'},
      {"use-legend", no_argument, nullptr, 'l'},
      {"use-grid", no_argument, nullptr, 'g'},
      {"output", required_argument, nullptr, 'o'},
      {nullptr, 0, nullptr, 0}};

  const char* short_opts = "f:t:x:y:lgo:";

  plotcpp::Plot2D plot;
  std::string csv_filename;
  std::string output_filename;
  bool first_line_has_labels = false;
  bool use_legend = false;

  int opt;
  int option_index = 0;
  while (true) {
    opt = getopt_long(argc, argv, short_opts, long_options, &option_index);

    if (opt == -1) {
      break;
    }

    switch (opt) {
      case 't':
        plot.SetTitle(std::string{optarg});
        break;

      case 'x':
        plot.SetXLabel(std::string{optarg});
        break;

      case 'y':
        plot.SetYLabel(std::string{optarg});
        break;

      case 'g':
        plot.SetGrid(true);
        break;

      case FIRST_LINE_LABEL_OPTION:
        first_line_has_labels = true;
        break;

      case 'l':
        use_legend = true;
        break;

      case 'f':
        csv_filename = std::string{optarg};
        break;

      case 'o':
        output_filename = std::string{optarg};
        break;

      default:
        break;
    }
  };

  if (csv_filename.empty()) {
    std::cout << "Error: specify input csv file" << std::endl;
  }

  const DataCollection collection =
      ParseCsv(csv_filename, DEFAULT_CSV_DELIMITER, first_line_has_labels);

  const std::size_t num_series = collection.series.size();
  if (num_series == 1) {
    plot.Plot(collection.series[0], {0, 0, 0}, 2, "");
  } else if (num_series > 1) {
    for (std::size_t i = 1; i < num_series; ++i) {
      plot.Plot(collection.series[0], collection.series[i], {0, 0, 0}, 2, "");
    }
  }

  if (use_legend && !collection.labels.empty()) {
    plot.SetLegend(collection.labels);
  }

  plot.Build();

  if (!output_filename.empty()) {
    plot.Save(output_filename);
  }

  return true;
}
