#pragma once

#include "effects.h"

class EFXCrossfade: public EFXBase {
    private:
        // CONFIG
        int     fade_time_ms    =    1000;  // Time in milliseconds this effect has to take

        bool    initial         =    true;  // If this is the first call for the current transition
        GLfloat fade_last       =    0.0f;  // The last alpha value of our image
        GLfloat fade_current    =    0.0f;  // The current alpha value of our image
        int     fade_offset_ms  =    0;     // Used in combination with "fade_time_ms" to calculate the new alpha value for the next frame

    public:
        void configure(const char *key, int value);
        bool render(GLuint old_texture, GLuint new_texture);
};
