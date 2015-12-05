#include "slide.h"
#include "utils.h"

#include <cstring>

EFXSlide::EFXSlide() {
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

EFXSlide::~EFXSlide() {
    delete [] tl;
    delete [] tr;
    delete [] bl;
    delete [] br; 
}

void EFXSlide::configure(const char *key, int value) {
    if ( strcmp( key, "slide_time_ms") == 0 )
        slide_time_ms = value;
}

bool EFXSlide::render(GLuint old_texture, GLuint new_texture) {
    // Very first frame so we reset/initialize all variables properly
    if ( initial ) {
        initial = false;
        pos_current = 0.0f;
        pos_offset_ms = (get_current_time_ms() % slide_time_ms);
    }

    if ( pos_offset_ms && pos_current < 2.0f ) {
        pos_current = (float) ( ( get_current_time_ms() - pos_offset_ms ) % slide_time_ms ) / slide_time_ms * 2;

        if (pos_current < pos_last) {
            pos_last       = 0.0f;
            pos_current    = 2.0f;
            pos_offset_ms  = 0;
        } else
            pos_last = pos_current;

        // Slide out old image
        tl[0] = -1.0f + pos_current;
        tr[0] =  1.0f + pos_current;
        bl[0] = -1.0f + pos_current;
        br[0] =  1.0f + pos_current;
        draw_image( old_texture, tl, tr, bl, br);

        // Slide in new image
        tl[0] = -3.0f + pos_current;
        tr[0] = -1.0f + pos_current;
        bl[0] = -3.0f + pos_current;
        br[0] = -1.0f + pos_current;
        draw_image( new_texture, tl, tr, bl, br);

        return false;
    }

    initial = true;

    return true;
}
