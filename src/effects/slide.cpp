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

void EFXSlide::configure(const char *key, void *value) {
    if ( strcmp( key, "slide_time_ms") == 0 )
        slide_time_ms = *(int*) value;
    else if ( strcmp( key, "slide_direction") == 0 )
        slide_direction = *(int*) value;
}

bool EFXSlide::render(GLuint old_texture, GLuint new_texture) {
    // Very first frame so we reset/initialize all variables properly
    if ( initial ) {
        initial = false;
        pos_current = 0.0f;
        pos_offset_ms = (get_current_time_ms() % slide_time_ms);
        random_direction = rand() % 4;
    }

    if ( pos_offset_ms && pos_current < 2.0f ) {
        pos_current = (float) ( ( get_current_time_ms() - pos_offset_ms ) % slide_time_ms ) / slide_time_ms * 2;

        if (pos_current < pos_last) {
            pos_last       = 0.0f;
            pos_current    = 2.0f;
            pos_offset_ms  = 0;
        } else
            pos_last = pos_current;

        switch (slide_direction) {
            case 0:
                left_to_right(old_texture, new_texture);
                break;
            case 1:
                right_to_left(old_texture, new_texture);
                break;
            case 2:
                top_to_bottom(old_texture, new_texture);
                break;
            case 3:
                bottom_to_top(old_texture, new_texture);
                break;
            case 4:
                random(random_direction, old_texture, new_texture);
                break;
        }

        return false;
    }

    initial = true;

    return true;
}

void EFXSlide::random(int direction, GLuint old_texture, GLuint new_texture) {
    switch (direction) {
        case 0:
            left_to_right(old_texture, new_texture);
            break;
        case 1:
            right_to_left(old_texture, new_texture);
            break;
        case 2:
            top_to_bottom(old_texture, new_texture);
            break;
        case 3:
            bottom_to_top(old_texture, new_texture);
            break;
    }
}

void EFXSlide::left_to_right(GLuint old_texture, GLuint new_texture) {
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
};

void EFXSlide::right_to_left(GLuint old_texture, GLuint new_texture) {
    // Slide out old image
    tl[0] = -1.0f - pos_current;
    tr[0] =  1.0f - pos_current;
    bl[0] = -1.0f - pos_current;
    br[0] =  1.0f - pos_current;
    draw_image( old_texture, tl, tr, bl, br);

    // Slide in new image
    tl[0] = 1.0f - pos_current;
    tr[0] = 3.0f - pos_current;
    bl[0] = 1.0f - pos_current;
    br[0] = 3.0f - pos_current;
    draw_image( new_texture, tl, tr, bl, br);
};

void EFXSlide::top_to_bottom(GLuint old_texture, GLuint new_texture) {
    // Slide out old image
    tl[1] = -1.0f + pos_current;
    tr[1] = -1.0f + pos_current;
    bl[1] =  1.0f + pos_current;
    br[1] =  1.0f + pos_current;
    draw_image( old_texture, tl, tr, bl, br);

    // Slide in new image
    tl[1] = -3.0f + pos_current;
    tr[1] = -3.0f + pos_current;
    bl[1] = -1.0f + pos_current;
    br[1] = -1.0f + pos_current;
    draw_image( new_texture, tl, tr, bl, br);
};

void EFXSlide::bottom_to_top(GLuint old_texture, GLuint new_texture) {
    // Slide out old image
    tl[1] = -1.0f - pos_current;
    tr[1] = -1.0f - pos_current;
    bl[1] =  1.0f - pos_current;
    br[1] =  1.0f - pos_current;
    draw_image( old_texture, tl, tr, bl, br);

    // Slide in new image
    tl[1] = 1.0f - pos_current;
    tr[1] = 1.0f - pos_current;
    bl[1] = 3.0f - pos_current;
    br[1] = 3.0f - pos_current;
    draw_image( new_texture, tl, tr, bl, br);
};