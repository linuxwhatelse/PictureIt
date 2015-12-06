#include "effects.h"

#include <sys/time.h>
#include <GL/gl.h>

#include <algorithm>

EFXBase::EFXBase() {
    this->tl = new GLfloat[2]();
    this->tr = new GLfloat[2]();
    this->bl = new GLfloat[2]();
    this->br = new GLfloat[2]();
}
EFXBase::~EFXBase() {
    delete[] this->tl;
    delete[] this->tr;
    delete[] this->bl;
    delete[] this->br;
}

long int EFXBase::get_current_time_ms() {
    struct timeval current_time;
    gettimeofday( &current_time, NULL );

    return current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
}


void EFXBase::draw_image(GLuint texture_id, GLfloat top_left[], GLfloat top_right[], GLfloat bottom_left[], GLfloat bottom_right[], float opacity) {
    if ( ! texture_id )
        return;

    if ( top_left )
        std::copy_n(top_left, 2, this->tl);
    else
        this->tl[0] = -1.0f; this->tl[1] = -1.0f;

    if ( top_right )
        std::copy_n(top_right, 2, this->tr);
    else
        this->tr[0] = 1.0f; this->tr[1] = -1.0f;

    if ( bottom_left )
        std::copy_n(bottom_left, 2, this->bl);
    else
        this->bl[0] = -1.0f; this->bl[1] = 1.0f;

    if ( bottom_right )
        std::copy_n(bottom_right, 2, this->br);
    else
        this->br[0] = 1.0f; this->br[1] = 1.0f;
    

    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glBindTexture( GL_TEXTURE_2D, texture_id );

    glColor4f( 1.0f, 1.0f, 1.0f, opacity );

    if ( this->window_width && this->window_height != -1 && this->image_width && this->image_height ) {
        switch (image_mode) {
            case MODE::CENTER:
                img_mode_center();
                break;
            case MODE::ZOOM:
                img_mode_zoom();
                break;
            case MODE::SCALE:
                img_mode_scale();
                break;

            default:
            case MODE::STRETCH:
                // Stretching doesn't need "special" values, therefore we do nothing
                break;
        }
    }

    glBegin( GL_TRIANGLES );
        glTexCoord2f( 0.0f, 0.0f ); glVertex2f( this->tl[0], this->tl[1] );  // Top Left
        glTexCoord2f( 1.0f, 0.0f ); glVertex2f( this->tr[0], this->tr[1] );  // Top Right
        glTexCoord2f( 1.0f, 1.0f ); glVertex2f( this->br[0], this->br[1] );  // Bottom Right
    glEnd();
    glBegin( GL_TRIANGLES );
        glTexCoord2f( 1.0f, 1.0f ); glVertex2f( this->br[0], this->br[1] );  // Bottom Right
        glTexCoord2f( 0.0f, 1.0f ); glVertex2f( this->bl[0], this->bl[1] );  // Bottom Left
        glTexCoord2f( 0.0f, 0.0f ); glVertex2f( this->tl[0], this->tl[1] );  // Top Left
    glEnd();

    glDisable( GL_TEXTURE_2D );
    glDisable( GL_BLEND );
}

void EFXBase::img_mode_center() {
    GLfloat diff_width  = 1.0f - ( (float)this->image_width  / (float)this->window_width  );
    GLfloat diff_height = 1.0f - ( (float)this->image_height / (float)this->window_height );

    this->tl[0] = this->tl[0] + diff_width;
    this->tr[0] = this->tr[0] - diff_width;
    this->bl[0] = this->bl[0] + diff_width;
    this->br[0] = this->br[0] - diff_width;

    this->tl[1] = this->tl[1] + diff_height;
    this->tr[1] = this->tr[1] + diff_height;
    this->bl[1] = this->bl[1] - diff_height;
    this->br[1] = this->br[1] - diff_height;
}

void EFXBase::img_mode_scale() {
    int new_image_width = this->image_width * ( (float)this->window_height / (float)this->image_height );
    GLfloat diff_width  = 1.0f - ( new_image_width * 1.0f / this->window_width );

    this->tl[0] = this->tl[0] + diff_width;
    this->tr[0] = this->tr[0] - diff_width;
    this->bl[0] = this->bl[0] + diff_width;
    this->br[0] = this->br[0] - diff_width;
}

void EFXBase::img_mode_zoom() {
    int new_image_height = this->image_height * ( (float)this->window_width / (float)this->image_width );
    GLfloat diff_height  = 1.0f - ( new_image_height * 1.0f / this->window_height );

    this->tl[1] = this->tl[1] + diff_height;
    this->tr[1] = this->tr[1] + diff_height;
    this->bl[1] = this->bl[1] - diff_height;
    this->br[1] = this->br[1] - diff_height;
}