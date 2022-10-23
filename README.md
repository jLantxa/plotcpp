# Introduction
## What is plotcpp?
`plotcpp` is a 2D plotting library for modern C++. Its purpose is to provide a variety of data visualization devices that are easy to use and integrate into your code. `plotcpp` is aimed at everyday/casual plotting needs as well as visualisation and debugging.

## How does it work?
`plotcpp` provides `Figure`s, which are the most general visualisation abstraction. A `Figure` contains one or more plots, which in turn can be configured in many obvious ways and ultimately built/rendered. A plot can be a pie chart, a histogram, a function plot, etc.

Internally, `plotcpp` represents all visual elements using an SVG description. This allows plots to be scalable as well as to potentially support an infinity of visual styles while looking nice.

## Requirements
At the moment, `plotcpp` requires at least C++17 because is uses `std::optional`. It is hard to find a balance between supporting a broader set of compiler standards and providing modern language features. As stated before, this library is targeted for use in modern C++ projects.

### Dependencies
`plotcpp` is based on the following dependencies:
* [libxml2](https://github.com/GNOME/libxml2)
* [fmt](https://fmt.dev/latest/index.html): Neither `clang` nor `gcc` support `<format>` yet, so fmt is a temporary workaround.

# About this project
## Supported features
### Plot2D
The `Plot2D` figure can plot single variable functions. A number of features are lacking for this figure type:
* Smart subdivision of axes
* Scatter plots

![Example](examples/numeric_plot2d.png)
![Example](examples/categorical_plot2d.png)

## Planned features
* Scatter plot
* Annotated heatmap
* Histogram
* Pie chart
* Grouping and subplot
