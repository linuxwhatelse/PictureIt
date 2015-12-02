#pragma once

#include <GL/gl.h>

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

    public:
        Spectrum(int spectrum_bar_count);
        ~Spectrum();

        float  spectrum_width             = 1.0f;   // Value ranging from 0.0f to 1.0f
        float  spectrum_position          = 1.0f;   // Value ranging from -1.0f to 1.0f
                                                    // -1.0f = The very top
                                                    //  0.0f = Center
                                                    //  1.0f = The very bottom
        float  spectrum_animation_speed   = 0.007f; // The smaler the value, the slower and smoother the animation
        bool   spectrum_mirror_vertical   = true;   // If the spectrum should be mirror vertically
        bool   spectrum_mirror_horizontal = false;  // If the spectrum should be mirror horizontally

        void draw_spectrum();
        void draw_bar( int i, GLfloat x1, GLfloat x2 );

        void audio_data( const float *audio_data, int audio_data_length );

        void set_bar_color( int pos, int r, int g, int b );
        void set_bar_color( int pos, float r, float g, float b );
};
