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

#include "fft.hpp"



FFT::FFT(const Config::FFT &fft_cfg) : fft_cfg(fft_cfg) {
    this->input = reinterpret_cast<float*>(
        fftwf_malloc(sizeof(float) * fft_cfg.get_size()));

    this->output = reinterpret_cast<fftwf_complex*>(
        fftwf_malloc(sizeof(fftwf_complex) * fft_cfg.get_output_size()));

    this->plan = fftwf_plan_dft_r2c_1d(fft_cfg.get_size(), this->input,
                                       this->output, FFTW_ESTIMATE);
}

FFT::~FFT() {
    fftwf_free(output);
    fftwf_free(this->input);
    fftwf_destroy_plan(this->plan);
}

template<typename T>
void FFT::calculate(Buffer<T>& buffer) {
    // find smallest value for window function
    size_t window_size = std::min(this->fft_cfg.get_size(), buffer.size);

    if (this->window.size() != window_size){
        calculate_window(window_size);
    }

    auto lock = buffer.lock();
    if(buffer.new_data){
        buffer.new_data = false;

        unsigned int i;
        for(i = 0; i < window_size; i++){
            // apply hann window with corrected factors (a * 2)
            this->input[i] = (static_cast<float>(buffer.v_buffer[i]) *
                              this->window[i]);
        }

        lock.unlock();

        // pad remainig values
        for(; i < fft_cfg.get_size(); i++){
            this->input[i] = 0;
        }

        // execute fft
        fftwf_execute(this->plan);
    }
}

void FFT::calculate_window(const size_t w_size) {
    this->window.resize(w_size);
    this->N_1 = 1.0 / (float) (w_size - 1);

    switch(this->fft_cfg.window) {
        case Config::FFT::Window::HANN:
            this->hann(w_size);
            break;
        case Config::FFT::Window::HAMMING:
            this->hamming(w_size);
            break;

        case Config::FFT::Window::EXACT_BLACKMAN:
            this->exact_blackman(w_size);
            break;
    }
}

void FFT::hann(const size_t w_size) {
    for (unsigned int i = 0; i < w_size; i++) {
        this->window[i] = 1.0 - cos(2.0 * M_PI * (float)i * this->N_1);
    }
}

void FFT::hamming(const size_t w_size) {
    for (unsigned int i = 0; i < w_size; i++) {
        this->window[i] = 0.54 - 0.46 * cos(2.0 * M_PI * (float)i * this->N_1);
    }
}

void FFT::exact_blackman(const size_t w_size) {
    float N_1 = 1.0 / (float) (w_size - 1);

    // compensated Blackman window constants
    const float a1 = 4620.0 / 3969.0;
    const float a2 = 715.0 / 3969.0;

    for (unsigned int i = 0; i < w_size; i++) {
        this->window[i] = (1.0 - a1 * cos(2 * M_PI * i * this->N_1) +
                           a2 * cos(4 * M_PI * i * this->N_1));
    }
}

template void FFT::calculate(Buffer<int16_t>&);
