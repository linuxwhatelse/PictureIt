#pragma once

#include "fft.hpp"
#include "config.hpp"
#include "program.hpp"
#include "gl_utils.hpp"

#include <memory>
#include <array>

using namespace PI;


class Spectrum {
    public:
        Spectrum(const Config::Spectrum&, const unsigned);
        // disable copy construction
        Spectrum(const Spectrum&) = delete;
        ~Spectrum(){};

        const unsigned int get_id() {
            return this->id;
        };

        void draw();
        void update_fft(FFT&);
        void update_fft(std::vector<std::shared_ptr<FFT>>&);
        void configure(const Config::Spectrum&);

    private:
        GL::Program sh_bars, sh_bars_pre, sh_lines;

        std::array<GL::VAO, 2> v_bars, v_bars_pre;

        GL::Buffer b_x, b_fft;
        std::array<GL::Buffer, 2> b_fb;

        unsigned tf_index = 0;
        size_t output_size, offset;
        unsigned id, bar_shader_id, channel;

        void init_bar_shader();
        void init_bars();
        void init_bar_pre_shader();
        void init_bars_pre();

        void resize_tf_buffers(const size_t);
        void resize_x_buffer(const size_t);
        void resize_fft_buffer(const size_t);
        void resize(const size_t);
        void set_transformation(const Config::Transformation&);
};
