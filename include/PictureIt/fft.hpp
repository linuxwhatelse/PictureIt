/*
 *  Copyright (C) 2016  Hannes Haberl
 *
 *  This file is part of GLMViz.
 *
 *  GLMViz is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GLMViz is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GLMViz.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <fftw3.h>

#include <vector>
#include <math.h>
#include <stdint.h>
#include <algorithm>

#include "config.hpp"
#include "buffer.hpp"

using namespace PI;


class FFT {
    public:
        FFT(const Config::FFT &fft_cfg);
        // disable copy construction
        FFT(const FFT&) = delete;
        ~FFT();

        float N_1 = 0;

        template<typename T>
        void calculate(Buffer<T>&);

        fftwf_complex* output;

        void hann(const size_t);
        void hamming(const size_t);
        void exact_blackman(const size_t);
    private:
        const Config::FFT &fft_cfg;

        float* input;
        fftwf_plan plan;
        std::vector<float> window;

        void calculate_window(const size_t);
};
