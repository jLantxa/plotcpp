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

#include "HistogramHandler.hpp"

#include <getopt.h>

#include <iostream>

#include "HistogramPlot.hpp"
#include "cli.hpp"
#include "csv.hpp"
#include "utility.hpp"

bool HistogramHandler::Run(int argc, char** argv) {
  static const struct option long_options[] = {
      {"filename", required_argument, nullptr, 'f'},
      {"num-bins", required_argument, nullptr, 'b'},
      {"title", required_argument, nullptr, 't'},
      {"x-label", required_argument, nullptr, 'x'},
      {"y-label", required_argument, nullptr, 'y'},
      {"use-grid", no_argument, nullptr, 'g'},
      {"output", required_argument, nullptr, 'o'},
      {nullptr, 0, nullptr, 0}};

  const char* short_opts = "f:t:x:y:go:";

  plotcpp::HistogramPlot plot;
  std::string csv_filename;
  std::string output_filename;
  unsigned int num_bins = 8;

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

      case 'b':
        num_bins = static_cast<unsigned int>(atol(optarg));
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
      ParseCsv(csv_filename, DEFAULT_CSV_DELIMITER, false);

  plotcpp::ColorSelector color_selector(plotcpp::color_tables::MUTED);
  // Histogram only plots one series
  const std::size_t num_series = collection.series.size();
  if (num_series > 0) {
    plot.Plot(collection.series[0], num_bins, color_selector.NextColor());
  }

  plot.Build();

  if (!output_filename.empty()) {
    plot.Save(output_filename);
  }

  return true;
}
