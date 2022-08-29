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

#ifndef _PLOTCPP_INCLUDE_PLOTCPP_HPP_
#define _PLOTCPP_INCLUDE_PLOTCPP_HPP_

#include <vector>

namespace plotcpp {

/** Internal real number type */
using Real = double;

struct Point {
    Real x, y;
};

namespace adaptor {
    /**
     * \brief Convert any numeric type to the internal real representation
     *
     * \tparam T Custom type
     * \param x Value
     * \return Real Conversion from T to Real
     */
    template <typename T>
    Real R(T x) {
        return static_cast<Real>(x);
    }

    /**
     * \brief Convert a vector of any numeric type to the internal real representation
     *
     * \tparam T Custom type
     * \param v Vector
     * \return std::vector<Real> Conversion from T to Real
     */
    template <typename T>
    std::vector<Real> R(std::vector<T> v) {
        const std::size_t size = v.size();

        std::vector<Real> real_vector;
        real_vector.resize(size);

        for (std::size_t i = 0; i < size; ++i) {
            real_vector[i] = R(v[i]);
        }

        return real_vector;
    }
}

}  // namespace plotcpp

#endif  // _PLOTCPP_INCLUDE_PLOTCPP_HPP_
