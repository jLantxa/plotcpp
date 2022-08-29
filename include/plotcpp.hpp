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
