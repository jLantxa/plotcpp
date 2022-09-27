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

#include <iostream>

#include <gtest/gtest.h>

#include "Plot2D.hpp"

using plotcpp::Plot2D;
using plotcpp::Real;

TEST(Plot2DTest, Constructor) {
	Plot2D plot1;
	EXPECT_EQ(plot1.Title(), "");
	EXPECT_EQ(plot1.Width(), 800);
	EXPECT_EQ(plot1.Height(), 600);
}

TEST(Plot2DTest, BuilderOptions) {
	Plot2D plot;

	plot.SetSize(123U, 456U);
	plot.SetTitle("Test figure 1234");
	plot.SetXLabel("X LABEL test");
	plot.SetYLabel("Y label TEST");

	EXPECT_EQ(plot.Width(), 123U);
	EXPECT_EQ(plot.Height(), 456U);
	EXPECT_EQ(plot.Title(), "Test figure 1234");
	EXPECT_EQ(plot.GetXLabel(), "X LABEL test");
	EXPECT_EQ(plot.GetYLabel(), "Y label TEST");

	EXPECT_FALSE(plot.GetXRange().has_value());
	EXPECT_FALSE(plot.GetYRange().has_value());

	std::pair<Real, Real> range {100.0f, 200.0f};

	plot.SetXRange(100.0f, 200.0f);
	ASSERT_TRUE(plot.GetXRange().has_value());
	EXPECT_EQ(plot.GetXRange().value(), range);
	plot.SetXRange(200.0f, 100.0f);
	EXPECT_EQ(plot.GetXRange().value(), range);

	plot.SetYRange(100.0f, 200.0f);
	ASSERT_TRUE(plot.GetYRange().has_value());
	EXPECT_EQ(plot.GetYRange().value(), range);
	plot.SetYRange(200.0f, 100.0f);
	EXPECT_EQ(plot.GetYRange().value(), range);
}
