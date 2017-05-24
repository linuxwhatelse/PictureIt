#pragma once

#include <string>
#include <vector>

#include <algorithm>
#include <cmath>


namespace PI {
    class Config {
    public:
        struct Transformation {
            float Xmin = -1;
            float Xmax = 1;
            float Ymin = -1;
            float Ymax = 1;
        };

        struct Color {
            float rgba[4];

            float& operator[](unsigned int index) {
                return rgba[index];
            };

            void normalize(const Color& c) {
                // convert screen color(CRT gamma) to sRGB
                const float inv_gamma = 1.0 / 2.2;

                for(int i = 0; i < 3; i ++){
                    rgba[i] = std::pow(c.rgba[i] / 255, inv_gamma);
                }
            };

            void normalize() {
                // convert screen color(CRT gamma) to sRGB
                const float inv_gamma = 1.0 / 2.2;
                for(int i = 0; i < 3; i ++) {
                    rgba[i] = std::pow(rgba[i] / 255, inv_gamma);
                }
            };
        };

        struct Transition {
            enum Type {
                NONE,
                CROSSFADE,
                SLIDE
            };

            Transition::Type type = Transition::Type::CROSSFADE;
            float speed = 100;
        };

        struct Texture {
            enum DisplayMode {
                STRETCH,
                CENTER,
                SCALE,
                ZOOM
            };

            std::string path;

            DisplayMode mode = DisplayMode::CENTER;

            Transition transition_in;
            Transition transition_out;

            Transformation pos;
            Color color = {0.0, 0.0, 0.0, 1.0};
        };

        struct PictureIt {
            int max_fps = 60;

            bool update_by_interval = true;
            unsigned int update_interval = 180;
            bool pick_random = true;

            Color bg_color = {0.0, 0.0, 0.0, 1.0};

            Transition transition;
            Texture::DisplayMode display_mode = Texture::DisplayMode::CENTER;

            const char *image_dir = nullptr;
            bool recursive = true;
        };

        struct FFT {
        private:
            size_t size = 8192L;
            size_t sample = 44100L;
            size_t output_size = 0;
            size_t buf_size = 0;

            float d_freq = 0;
            float scale = 0;
            int duration = 100;

        public:
            enum Window {
                HANN,
                HAMMING,
                EXACT_BLACKMAN
            };

            FFT(size_t fft_size = 8192L) {
                set_size(fft_size);
            }

            Window window = Window::EXACT_BLACKMAN;

            void set_size(size_t fft_size) {
                size = fft_size;

                output_size = size / 2 + 1;
                set_sample(sample);
            }

            void set_sample(size_t sample_rate) {
                sample = sample_rate;
                d_freq = (float) sample / (float) size;

                buf_size = sample * duration / 1000;
                scale = 1.0f / ((float) (buf_size / 2 + 1) * 32768.0f);

            }

            void set_duration(int duration) {
                duration = duration;
                set_sample(sample);
            }

            size_t get_size() const { return size; }
            size_t get_sample() const { return sample; }
            size_t get_output_size() const { return output_size; }
            size_t get_buffer_size() const { return buf_size; }
            float get_d_freq() const { return d_freq; }
            float get_scale() const { return scale; }
        };



        struct Spectrum {
        private:
            FFT fft;

            int channel = 1;

            float slope = 0.5;
            float offset = 1.0;
            float gravity = 4.0 / 3600;
            float min_db = -31;
            float max_db = 2.0;

            int f_start = 20;
            int f_stop = 750;

            int output_size = 100;
            int data_offset = 0;

        public:
            Spectrum(const FFT &fft_cfg) : fft(fft_cfg) {
                set_gravity(4.0);
                set_db_range(min_db, max_db);
                set_freq_range(f_start, f_stop);
            }

            Color top_color = {1.0, 1.0, 1.0, 1.0};
            Color bot_color = {1.0, 1.0, 1.0, 1.0};

            Transformation pos;

            float gradient = 1.0;
            float bar_width = 0.75;

            void set_gravity(float grav) {
                // Artificial value (Originates from grav / (fps * fps))
                // Using the fps-limiting number would affect the gravity
                // Which we don't want
                gravity = grav / 3600;
            };

            void set_freq_range(unsigned int start, unsigned int stop) {
                if (stop <= start)
                    return;

                f_start = start;
                f_stop = stop;

                data_offset = std::floor((float) f_start / fft.get_d_freq());
                output_size = std::ceil((float) f_stop / fft.get_d_freq()) - (data_offset - 1);
                output_size = std::min(output_size, (int) fft.get_size() - data_offset);
            };

            void set_db_range(float min, float max) {
                if (max <= min)
                    return;

                min_db = min;
                max_db = max;

                float max_n = max_db * 0.05;
                float min_n = min_db * 0.05;

                slope = -2.0 / (min_n - max_n);
                offset = 1.0 - slope * max_n;
            };

            FFT get_fft() const { return fft; };
            int get_channel() const { return channel; };
            float get_slope() const { return slope; };
            float get_offset() const { return offset; };
            float get_gravity() const { return gravity; };
            int get_output_size() const { return output_size; }
            int get_data_offset() const { return data_offset; }
            float get_min_db() const { return min_db; };
            float get_max_db() const { return max_db; };
        };
    };
};
