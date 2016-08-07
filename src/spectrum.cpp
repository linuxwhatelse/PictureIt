#include "spectrum.hpp"

#include <stdio.h>
#include <math.h>
#include <algorithm>


#define RING_BUF_SIZE 8192*30
#define FFT_FRAME_SIZE 512


Spectrum::Spectrum(int spectrum_bar_count) : 
  audio_ring_buf(RING_BUF_SIZE), 
  fft_frame_size(FFT_FRAME_SIZE) {
    this->spectrum_bar_count = spectrum_bar_count;   

	asplib::CSpectrumVisProcessorConfigurator vis_processor_configurator;
    ASPLIB_ERR asplib_error = this->vis_processor.Create(vis_processor_configurator, this->spectrum_bar_count, this->fft_frame_size);
    if (asplib_error == ASPLIB_ERR_NO_ERROR) {
        this->vis_processor_initialized = true;
    }
    this->vis_audio_data = new float[this->fft_frame_size*2];

    this->bar_heights  = new GLfloat[this->spectrum_bar_count];
    this->cbar_heights = new GLfloat[this->spectrum_bar_count*2];
    this->pbar_heights = new GLfloat[this->spectrum_bar_count*4];


    std::fill_n(this->bar_heights, this->spectrum_bar_count, 0.0f);
    std::fill_n(this->cbar_heights, this->spectrum_bar_count, 1.0f);
    std::fill_n(this->pbar_heights, this->spectrum_bar_count, 0.0f);
	std::fill_n(this->vis_audio_data, this->spectrum_bar_count*2, 0.0f);


    this->spectrum_colors = new GLfloat[this->spectrum_bar_count*3];
    for( int i = 0; i < this->spectrum_bar_count; i++ ) {
        this->spectrum_colors[ 3*i ]   = 1.0f;
        this->spectrum_colors[ 3*i+1 ] = 1.0f;
        this->spectrum_colors[ 3*i+2 ] = 1.0f;
    }
}

Spectrum::~Spectrum() {
    this->vis_processor.Destroy();

    delete[] this->vis_audio_data;

    delete[] this->bar_heights;
    delete[] this->cbar_heights;
    delete[] this->pbar_heights;
    delete[] this->spectrum_colors;
}

void Spectrum::audio_data(const float *audio_data, int audio_data_length) {
    this->audio_ring_buf.write((float*)audio_data, audio_data_length*2);
}

void Spectrum::draw_bar( int i, GLfloat pos_x1, GLfloat pos_x2 ) {
    if ( ::fabs( cbar_heights[i] - bar_heights[i] ) > 0 ) {
        // The bigger the difference between the current and previous heights, the faster
        // we want the bars to move.
        // The "10.0" is a random value I choose after some testing.
        float gravity = ::fabs( this->cbar_heights[i] - this->pbar_heights[i] ) / 10.0;

        if ( this->cbar_heights[i] < this->bar_heights[i] )
            this->cbar_heights[i] += this->spectrum_animation_speed + gravity;
        else
            this->cbar_heights[i] -= this->spectrum_animation_speed + gravity;
    }
    this->pbar_heights[i] = this->bar_heights[i];

    auto&& draw_bar = [&](GLfloat x1, GLfloat x2, GLfloat y) {
        glColor3f( this->spectrum_colors[ 3*i ], this->spectrum_colors[ 3*i+1 ], this->spectrum_colors[ 3*i+2 ] );

        y = -y;

        glBegin(GL_TRIANGLES);
            glVertex2f( x1, y );                           // Top Left
            glVertex2f( x2, y );                           // Top Right
            glVertex2f( x2, this->spectrum_position_vertical );  // Bottom Right
        glEnd();
        glBegin(GL_TRIANGLES);
            glVertex2f( x2, this->spectrum_position_vertical );  // Bottom Right
            glVertex2f( x1, this->spectrum_position_vertical );  // Bottom Left
            glVertex2f( x1, y );                           // Top Left
        glEnd();

        if ( this->spectrum_mirror_horizontal ) {
            x1 = x1 - ( this->spectrum_position_horizontal * 2 );
            x2 = x2 - ( this->spectrum_position_horizontal * 2 );

            glBegin(GL_TRIANGLES);
                glVertex2f( -x2, y );                           // Top Left
                glVertex2f( -x1, y );                           // Top Right
                glVertex2f( -x1, this->spectrum_position_vertical );  // Bottom Right
            glEnd();
            glBegin(GL_TRIANGLES);
                glVertex2f( -x1, this->spectrum_position_vertical );  // Bottom Right
                glVertex2f( -x2, this->spectrum_position_vertical );  // Bottom Left
                glVertex2f( -x2, y );                           // Top Left
            glEnd();
        }
    };

    GLfloat y = this->cbar_heights[i];
    pos_x1 = pos_x1 + this->spectrum_position_horizontal;
    pos_x2 = pos_x2 + this->spectrum_position_horizontal;

    if ( this->spectrum_flip_vertical ) {
        y = -this->spectrum_position_vertical - this->cbar_heights[i];
        draw_bar(pos_x1, pos_x2, -y);
    } else {
        y = this->spectrum_position_vertical - this->cbar_heights[i];
        draw_bar(pos_x1, pos_x2, y);
    }

    if ( this->spectrum_mirror_vertical ) {
        if ( this->spectrum_flip_vertical ) {
            y = this->spectrum_position_vertical - this->cbar_heights[i];
            draw_bar(pos_x1, pos_x2, y);
        } else {
            y = -this->spectrum_position_vertical - this->cbar_heights[i];
            draw_bar(pos_x1, pos_x2, -y);
        }
    }
}

void Spectrum::draw_spectrum() {
    // The mirrored bars will get drawn within the "draw_bars".
    // This ensures the exact same height-value for both
    // the left and the (mirrored) right bar
    glPushMatrix();
        GLfloat x1, x2, bar_width, gap;

        if ( this->spectrum_mirror_horizontal ) {
            bar_width = this->spectrum_width / this->spectrum_bar_count;
        } else {
            // Because the coord-system goes from -1.0 to 1.0, we have
            // to take spectrum_width * 2 in case of disabling the mirrored
            // part
            bar_width = (this->spectrum_width * 2) / this->spectrum_bar_count;
        }

		if (this->audio_ring_buf.get_StoredSamples() >= this->fft_frame_size*2) {
			if (this->audio_ring_buf.read(this->vis_audio_data, this->fft_frame_size*2) >= this->fft_frame_size*2)
			{
				for (int i = 0; i < this->fft_frame_size; i++) {
					this->vis_audio_data[i] = (this->vis_audio_data[i*2] + this->vis_audio_data[i*2 + 1])*0.5f;
				}
				if (this->vis_processor_initialized) {
					this->vis_processor.Process(this->vis_audio_data, this->cbar_heights);
				}
				else {
					// TODO: error handling
				}

				// scale spectrum data
				for (int i = 0; i < this->spectrum_bar_count; i++) {
            this->cbar_heights[i] = 140.0f + this->cbar_heights[i];
            if (this->cbar_heights[i] <= 0.0f)
            {
                this->cbar_heights[i] = 0.0f;
            }
            else
            {
                this->cbar_heights[i] /= 140.0f;
            }
				}

				for ( int i = 1; i <= this->spectrum_bar_count; i++ ) {
					// calculate position
					x1 = ( -this->spectrum_width ) + ( i * bar_width ) - bar_width;
					x2 = ( -this->spectrum_width ) + ( i * bar_width );

					// "add" a gap (which is 1/4 of the initial bar_width)
					// to both the left and right side of the bar
					gap = ( bar_width / 4 );
					x1 = x1 + gap;
					x2 = x2 - gap;

					if ( this->spectrum_flip_horizontal ) {
						x1 = -x1;
						x2 = -x2;
					}

					draw_bar( (i-1), x1, x2 );
				}
			}
		}
    glPopMatrix();
}

void Spectrum::set_bar_color( int pos, float r, float g, float b ) {
    this->spectrum_colors[ 3*pos ]   = r;
    this->spectrum_colors[ 3*pos+1 ] = g;
    this->spectrum_colors[ 3*pos+2 ] = b;
}
