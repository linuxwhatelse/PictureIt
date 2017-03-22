#pragma once

#include "effects.hpp"

class EFXFlip: public EFXBase {
    private:
        // CONFIG
        int     flip_time_ms   =    1500;  // Time in milliseconds this effect has to take

        bool    initial         =    true;  // If this is the first call for the current transition
        GLfloat pos_last        =    0.0f;  // The last position for our image
        GLfloat pos_current     =    0.0f;  // The current position for our image
        int     pos_offset_ms   =    0;     // Used in combination with "fade_time_ms" to calculate the new position for the next frame

        GLfloat *tl;  // The images top left corner position
        GLfloat *tr;  // The images top right corner position
        GLfloat *bl;  // The images bottom left corner position
        GLfloat *br;  // The images bottom right corner position

    public:
        EFXFlip();
        ~EFXFlip();

        void configure(const char *key, void *value);
        bool render(GLuint old_texture, GLuint new_texture);
};
