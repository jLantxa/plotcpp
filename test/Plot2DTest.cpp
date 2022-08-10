/*
 * plotcpp is a 2D plotting library for modern C++
 * Copyright (C) 2022  Javier Lancha Vázquez <javier.lancha@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <gtest/gtest.h>

#include "Plot2D.hpp"

using plotcpp::Plot2D;
using plotcpp::Real;

TEST(Plot2DTest, Constructor) {
	Plot2D<float, float> plot1;
	EXPECT_EQ(plot1.Title(), "");
	EXPECT_EQ(plot1.GetID(), 1U);
	EXPECT_EQ(plot1.Width(), 400);
	EXPECT_EQ(plot1.Height(), 300);

	Plot2D<float, float> plot2;
	EXPECT_EQ(plot2.GetID(), 2U);
}

TEST(Plot2DTest, BuilderOptions) {
	Plot2D<float, float> plot;

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
