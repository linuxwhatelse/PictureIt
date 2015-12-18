#include "spectrum.h"

#include <stdio.h>
#include <math.h>
#include <algorithm>

Spectrum::Spectrum(int spectrum_bar_count) {
    this->spectrum_bar_count = spectrum_bar_count;

    this->bar_heights  = new GLfloat[spectrum_bar_count];
    this->cbar_heights = new GLfloat[spectrum_bar_count];
    this->pbar_heights = new GLfloat[spectrum_bar_count];

    std::fill_n(this->bar_heights,  spectrum_bar_count, 0.0f);
    std::fill_n(this->cbar_heights, spectrum_bar_count, 0.0f);
    std::fill_n(this->pbar_heights, spectrum_bar_count, 0.0f);

    this->spectrum_colors = new GLfloat[spectrum_bar_count*3];
    for( int i = 0; i < spectrum_bar_count; i++ ) {
        this->spectrum_colors[ 3*i ]   = 1.0f;
        this->spectrum_colors[ 3*i+1 ] = 1.0f;
        this->spectrum_colors[ 3*i+2 ] = 1.0f;
    }
}

Spectrum::~Spectrum() {
    delete[] this->bar_heights;
    delete[] this->cbar_heights;
    delete[] this->pbar_heights;
    delete[] this->spectrum_colors;
}

void Spectrum::audio_data(const float *audio_data, int audio_data_length) {
    for(int i = 0; i < spectrum_bar_count; i++)
        bar_heights[i] = audio_data[i];
    /**
    ToDo: Use a library for FFT and windowing
    - The values in :this->bar_heights: will be used to draw the bars and the values have to be between 0.0f and 1.0f
      (1.0f would mean the bar goes all the way up to the screen meaning we should define a max value)
    - :this->spectrum_bar_count: tells us how many bars will be drawn
    - :this->cbar_heights / this->pbar_heights: are used by :draw_spectrum: to smoothen the animation and shall NOT be used here

    So all that this function has to do is write proper values into :this->bar_heights: whenever it gets called
    */
}

void Spectrum::draw_bar( int i, GLfloat pos_x1, GLfloat pos_x2 ) {
    if ( ::fabs( cbar_heights[i] - bar_heights[i] ) > 0 ) {
        // The bigger the difference between the current and previous heights, the faster
        // we want the bars to move.
        // The "10.0" is a random value I choose after some testing.
        float gravity = ::fabs( cbar_heights[i] - pbar_heights[i] ) / 10.0;

        if ( cbar_heights[i] < bar_heights[i] )
            cbar_heights[i] += spectrum_animation_speed + gravity;
        else
            cbar_heights[i] -= spectrum_animation_speed + gravity;
    }

    pbar_heights[i] = bar_heights[i];

    auto&& draw_bar = [&](GLfloat x1, GLfloat x2, GLfloat y) {
        glColor3f( spectrum_colors[ 3*i ], spectrum_colors[ 3*i+1 ], spectrum_colors[ 3*i+2 ] );

        glBegin(GL_TRIANGLES);
            glVertex2f( x1, y );                           // Top Left
            glVertex2f( x2, y );                           // Top Right
            glVertex2f( x2, spectrum_position_vertical );  // Bottom Right
        glEnd();
        glBegin(GL_TRIANGLES);
            glVertex2f( x2, spectrum_position_vertical );  // Bottom Right
            glVertex2f( x1, spectrum_position_vertical );  // Bottom Left
            glVertex2f( x1, y );                           // Top Left
        glEnd();

        if ( spectrum_mirror_vertical ) {
            x1 = x1 - ( spectrum_position_horizontal * 2 );
            x2 = x2 - ( spectrum_position_horizontal * 2 );

            // This is the mirrored part on the right side
            glBegin(GL_TRIANGLES);
                glVertex2f( -x2, y );                           // Top Left
                glVertex2f( -x1, y );                           // Top Right
                glVertex2f( -x1, spectrum_position_vertical );  // Bottom Right
            glEnd();
            glBegin(GL_TRIANGLES);
                glVertex2f( -x1, spectrum_position_vertical );  // Bottom Right
                glVertex2f( -x2, spectrum_position_vertical );  // Bottom Left
                glVertex2f( -x2, y );                           // Top Left
            glEnd();
        }
    };

    GLfloat y;
    pos_x1 = pos_x1 + spectrum_position_horizontal;
    pos_x2 = pos_x2 + spectrum_position_horizontal;

    if ( this->spectrum_flip_horizontal ) {
        y = -spectrum_position_vertical - cbar_heights[i];
        draw_bar(pos_x1, pos_x2, -y);
    } else {
        y = spectrum_position_vertical - cbar_heights[i];
        draw_bar(pos_x1, pos_x2, y);
    }

    if ( spectrum_mirror_horizontal ) {
        if ( this->spectrum_flip_horizontal ) {
            y = spectrum_position_vertical - cbar_heights[i];
            draw_bar(pos_x1, pos_x2, y);
        } else {
            y = -spectrum_position_vertical - cbar_heights[i];
            draw_bar(pos_x1, pos_x2, -y);
        }
    }
}

void Spectrum::draw_spectrum() {
    // The mirrored bars will get drawn within the "draw_bars".
    // This ensures the exact same height-value for both
    // the left and the (mirrored) right bar
    glPushMatrix();
        GLfloat x1, x2, bar_width;

        if ( spectrum_mirror_vertical )
            bar_width = spectrum_width / spectrum_bar_count;
        else
            // Because the coord-system goes from -1.0 to 1.0, we have
            // to take spectrum_width * 2 in case of disabling the mirrored
            // part
            bar_width = (spectrum_width * 2) / spectrum_bar_count;

        for ( int i = 1; i <= spectrum_bar_count; i++ ) {
            // calculate position
            x1 = ( spectrum_width * -1 ) + ( i * bar_width ) - bar_width;
            x2 = ( spectrum_width * -1 ) + ( i * bar_width );

            // "add" a gap (which is 1/4 of the initial bar_width)
            // to both the left and right side of the bar
            x1 = x1 + ( bar_width / 4 );
            x2 = x2 - ( bar_width / 4 );

            if ( this->spectrum_flip_vertical ) {
                x1 = -x1;
                x2 = -x2;
            }

            draw_bar( (i-1), x1, x2 );
        }
    glPopMatrix();
}

void Spectrum::set_bar_color( int pos, float r, float g, float b ) {
    this->spectrum_colors[ 3*pos ]   = r;
    this->spectrum_colors[ 3*pos+1 ] = g;
    this->spectrum_colors[ 3*pos+2 ] = b;
}
