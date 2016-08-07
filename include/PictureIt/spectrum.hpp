#pragma once

#if defined(TARGET_WINDOWS)
  #include <Windows.h>
#endif
#include <GL/gl.h>

#include <asplib/SpectrumVisProcessor/asplib_SpectrumVisProcessor.hpp>
#include <asplib/Core/Buffers/asplib_TRingBuffer.h>


class Spectrum {

    private:
        int     spectrum_bar_count; // Amount of single bars to display
        GLfloat *bar_heights;       // FFT'ed heights for each bar
        GLfloat *pbar_heights;      // Previous FFT'ed value. (used to calculate gravity.
                                    // The bigger the difference to :bar_heights:, the faster the bars will move)
        GLfloat *cbar_heights;      // Used to smoothen the animation on a "per frame" basis
        GLfloat *spectrum_colors;   // Will be the size of :spectrum_bar_count: * 3
                                    //   spectrum_colors[3*pos]   = r;
                                    //   spectrum_colors[3*pos+1] = g;
                                    //   spectrum_colors[3*pos+2] = b;


        float *vis_audio_data;
        bool vis_audio_data_initialized = false;
        int frame_size                  = 0;


        bool vis_processor_initialized  = false;

        asplib::TRingBuffer<float>                audio_ring_buf;
        asplib::CSpectrumVisProcessor             vis_processor;

    public:
        /*!
         * @brief Create a new instance of the Spectrum
         * @param spectrum_bar_count Amount of bars the spectrum should have. This excludes the mirrored parts
         */
        Spectrum(int spectrum_bar_count);
        ~Spectrum();

         /*!
          * @brief Amount fft samples to calculate a spectrum
          */
        const unsigned int fft_frame_size;

        /*!
         * @brief Width of the spectrum ranging from 0.0f to 1.0f
         */
        float spectrum_width = 1.0f;

        /*!
         * @brief Vertical position of the spectrum ranging from -1.0f (the very top) to 1.0f (the very bottom)
         * @details The position can go beyond -1.0f and 1.0f which would move the spectrum outside the visible boundaries
         */
        float spectrum_position_vertical = 1.0f;

        /*!
         * @brief Horizontal position of the spectrum ranging from -1.0f to 1.0f where the center of the spectrum is the anchor point.
         * This means that setting the position to -1.0f still shows half of the spectrum in screen
         * @details The position can go beyond -1.0f and 1.0f which would move the spectrum outside the visible boundaries
         */
        float spectrum_position_horizontal = 0.0f;

        /*!
         * @brief Animation speed of the spectrum
         * @details the smaler the value, the slower but smoother the animation
         */
        float spectrum_animation_speed = 0.007f;

        /*!
         * @brief Whether the spectrum should be mirror on the vertical axis or not
         */
        bool spectrum_mirror_vertical = true;

        /*!
         * @brief Whether the spectrum should be mirror on the horizontal axis or not
         */
        bool spectrum_mirror_horizontal = false;

        /*!
         * @brief Whether the spectrum should be fliped on the vertical axis or not
         */
        bool spectrum_flip_vertical = false;

        /*!
         * @brief Whether the spectrum should be flipped on the horizontal axis or not
         */
        bool spectrum_flip_horizontal = false;


        /*!
         * @brief Draws the spectrum and takes all previously defined values into account
         */
        void draw_spectrum();

        /*!
         * @brief Draws a single bar at a specified position (including the mirrored components if enabled)
         * @param i As the height is defined by the audio data received, :i: has to represent a valid index of :bar_heights:
         * @param pos_x1 The starting point of the bar (the left side)
         * @param pos_x2 The end point of the bar (the right side)
         */
        void draw_bar( int i, GLfloat pos_x1, GLfloat pos_x2 );

        /*!
         * @brief Takes raw audio data and applies both windowing and FFT
         * @param audio_data Array of floats holding the raw audio data
         * @param audio_data_length length of :audio_data:
         */
        void audio_data( const float *audio_data, int audio_data_length );

        /*!
         * @brief Sets the color for a indiviudal bar
         * @param pos The bar position from left to right starting from 0 up to :spectrum_bar_count: defined with the constructor
         * @param Amount of red ranging from 0.0f to 1.0f
         * @param Amount of green ranging from 0.0f to 1.0f
         * @param Amount of blue ranging from 0.0f to 1.0f
         */
        void set_bar_color( int pos, float r, float g, float b );
};
