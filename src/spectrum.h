#pragma once

#include <GL/gl.h>

class Spectrum {

    private:
        int      spectrum_bar_count;  // Amount of single bars to display
        GLfloat  *bar_heights;        // FFT'ed heights for each bar
        GLfloat  *pbar_heights;       // Previous FFT'ed value. (used to calculate gravity.
                                      // The bigger the difference to :bar_heights:, the faster the bars will move)
        GLfloat  *cbar_heights;       // Used to smoothen the animation on a "per frame" basis

    public:
        Spectrum(int spectrum_bar_count);
        ~Spectrum();

        float  spectrum_width             = 1.0f;    // Value ranging from 0.0f to 1.0f
        int    spectrum_bar_width         = 3;       // Value ranging from 1 to n
        float  spectrum_position          = 1.0f;    // Value ranging from -1.0f to 1.0f
                                                     // -1.0f = The very top
                                                     //  0.0f = Center
                                                     //  1.0f = The very bottom
        float  spectrum_animation_speed   = 0.007f;  // The smaler the value, the slower and smoother the animation
        bool   spectrum_mirror_vertical   = true;    // If the spectrum should be mirror vertically
        bool   spectrum_mirror_horizontal = false;    // If the spectrum should be mirror horizontally

        void draw_spectrum();
        void draw_bars( int i, GLfloat x1, GLfloat x2 );

        void audio_data( const float *audio_data, int audio_data_length );

};
