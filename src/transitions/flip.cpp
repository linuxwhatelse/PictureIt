#include <iostream>

#include "effects/flip.hpp"
#include "utils.hpp"

#include <cstring>

EFXFlip::EFXFlip() {
    tl = new GLfloat[2];
    tl[0] = -1.0f;
    tl[1] = -1.0f;

    tr = new GLfloat[2];
    tr[0] = 1.0f;
    tr[1] = -1.0f;

    bl = new GLfloat[2];
    bl[0] = -1.0f;
    bl[1] =  1.0f;

    br = new GLfloat[2];
    br[0] = 1.0f;
    br[1] = 1.0f;
}

EFXFlip::~EFXFlip() {
    delete [] tl;
    delete [] tr;
    delete [] bl;
    delete [] br; 
}

void EFXFlip::configure(const char *key, void *value) {
    if ( strcmp( key, "flip_time_ms") == 0 )
        flip_time_ms = *(int*) value;
}

bool EFXFlip::render(GLuint old_texture, GLuint new_texture) {
    // Very first frame so we reset/initialize all variables properly
    if ( initial ) {
        initial = false;
        pos_current = 0.0f;
        pos_offset_ms = (PI_UTILS::get_time_in_ms() % flip_time_ms);
    }

    if ( pos_offset_ms && pos_current < 1.0f ) {
        pos_current = (float) ( ( PI_UTILS::get_time_in_ms() - pos_offset_ms ) % flip_time_ms ) / flip_time_ms;

        if (pos_current < pos_last) {
            pos_last       = 0.0f;
            pos_current    = 0.0f;
            pos_offset_ms  = 0;
        } else {
            pos_last = pos_current;
        }

        // Flip out old image
        if ( pos_current > 0.0f && pos_current <= 0.5f ) {
            tl[1] = -1.0f + pos_current;
            tr[1] = -1.0f + pos_current;

            draw_image( old_texture, false, tl, tr, bl, br);
        }

        // Flip in new image
        if ( pos_current >= 0.5f ) {
            tl[1] = 0.0f + pos_current;
            tr[1] = 0.0f + pos_current;
            draw_image( new_texture, true, tl, tr, bl, br);
        }

        if ( pos_current == 0.0f ) {
            draw_image( new_texture, true, tl, tr, bl, br);
        }

        return false;
    }

    initial = true;

    return true;
}
