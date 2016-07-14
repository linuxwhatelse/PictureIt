#include "effects/slide.hpp"
#include "utils.hpp"

#include <cstring>

EFXSlide::EFXSlide() {
    this->tl = new GLfloat[2];
    this->tl[0] = -1.0f;
    this->tl[1] = -1.0f;

    this->tr = new GLfloat[2];
    this->tr[0] = 1.0f;
    this->tr[1] = -1.0f;

    this->bl = new GLfloat[2];
    this->bl[0] = -1.0f;
    this->bl[1] =  1.0f;

    this->br = new GLfloat[2];
    this->br[0] = 1.0f;
    this->br[1] = 1.0f;
}

EFXSlide::~EFXSlide() {
    delete [] this->tl;
    delete [] this->tr;
    delete [] this->bl;
    delete [] this->br; 
}

void EFXSlide::configure(const char *key, void *value) {
    if ( strcmp( key, "slide_time_ms") == 0 )
        this->slide_time_ms = *(int*) value;

    else if ( strcmp( key, "slide_direction") == 0 )
        this->slide_direction = *(int*) value;
}

bool EFXSlide::render(GLuint old_texture, GLuint new_texture) {
    // Very first frame so we reset/initialize all variables properly
    if ( this->initial ) {
        this->initial = false;
        this->pos_current = 0.0f;
        this->pos_offset_ms = (get_current_time_ms() % this->slide_time_ms);
        this->random_direction = rand() % 4;
    }

    if ( this->pos_offset_ms && this->pos_current < 2.0f ) {
        this->pos_current = (float) ( ( get_current_time_ms() - this->pos_offset_ms ) % this->slide_time_ms ) / this->slide_time_ms * 2;

        if ( this->pos_current < this->pos_last ) {
            this->pos_last       = 0.0f;
            this->pos_current    = 2.0f;
            this->pos_offset_ms  = 0;
        } else
            this->pos_last = this->pos_current;

        if ( this->slide_direction == 4 ) {
            slide( random_direction, old_texture, new_texture );
        } else {
            slide( this->slide_direction, old_texture, new_texture );
        }

        return false;
    }

    this->initial = true;

    return true;
}

void EFXSlide::slide(int direction, GLuint old_texture, GLuint new_texture) {
    switch ( direction ) {
        case 0:
            left_to_right( old_texture, new_texture );
            break;
        case 1:
            right_to_left( old_texture, new_texture );
            break;
        case 2:
            top_to_bottom( old_texture, new_texture );
            break;
        case 3:
            bottom_to_top( old_texture, new_texture );
            break;
    }
}

void EFXSlide::left_to_right(GLuint old_texture, GLuint new_texture) {
    // Slide out old image
    this->tl[0] = -1.0f + this->pos_current;
    this->tr[0] =  1.0f + this->pos_current;
    this->bl[0] = -1.0f + this->pos_current;
    this->br[0] =  1.0f + this->pos_current;
    draw_image( old_texture, false, this->tl, this->tr, this->bl, this->br);

    // Slide in new image
    this->tl[0] = -3.0f + this->pos_current;
    this->tr[0] = -1.0f + this->pos_current;
    this->bl[0] = -3.0f + this->pos_current;
    this->br[0] = -1.0f + this->pos_current;
    draw_image( new_texture, true, this->tl, this->tr, this->bl, this->br);
};

void EFXSlide::right_to_left(GLuint old_texture, GLuint new_texture) {
    // Slide out old image
    this->tl[0] = -1.0f - this->pos_current;
    this->tr[0] =  1.0f - this->pos_current;
    this->bl[0] = -1.0f - this->pos_current;
    this->br[0] =  1.0f - this->pos_current;
    draw_image( old_texture, false, this->tl, this->tr, this->bl, this->br);

    // Slide in new image
    this->tl[0] = 1.0f - this->pos_current;
    this->tr[0] = 3.0f - this->pos_current;
    this->bl[0] = 1.0f - this->pos_current;
    this->br[0] = 3.0f - this->pos_current;
    draw_image( new_texture, true, this->tl, this->tr, this->bl, this->br);
};

void EFXSlide::top_to_bottom(GLuint old_texture, GLuint new_texture) {
    // Slide out old image
    this->tl[1] = -1.0f + this->pos_current;
    this->tr[1] = -1.0f + this->pos_current;
    this->bl[1] =  1.0f + this->pos_current;
    this->br[1] =  1.0f + this->pos_current;
    draw_image( old_texture, false, this->tl, this->tr, this->bl, this->br);

    // Slide in new image
    this->tl[1] = -3.0f + this->pos_current;
    this->tr[1] = -3.0f + this->pos_current;
    this->bl[1] = -1.0f + this->pos_current;
    this->br[1] = -1.0f + this->pos_current;
    draw_image( new_texture, true, this->tl, this->tr, this->bl, this->br);
};

void EFXSlide::bottom_to_top(GLuint old_texture, GLuint new_texture) {
    // Slide out old image
    this->tl[1] = -1.0f - this->pos_current;
    this->tr[1] = -1.0f - this->pos_current;
    this->bl[1] =  1.0f - this->pos_current;
    this->br[1] =  1.0f - this->pos_current;
    draw_image( old_texture, false, this->tl, this->tr, this->bl, this->br);

    // Slide in new image
    this->tl[1] = 1.0f - this->pos_current;
    this->tr[1] = 1.0f - this->pos_current;
    this->bl[1] = 3.0f - this->pos_current;
    this->br[1] = 3.0f - this->pos_current;
    draw_image( new_texture, true, this->tl, this->tr, this->bl, this->br);
};
