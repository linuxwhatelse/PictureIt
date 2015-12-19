#pragma once

#include "effects.h"

class EFXSlide: public EFXBase {
    private:
        /*!
         * @brief Time in milliseconds the fade effect should take
        */
        int slide_time_ms = 2500;

        /*!
         * @biref Defines the direction to slide in/out new/old images
         * 0 = left to right
         * 1 = right to left
         * 2 = top to bottom
         * 3 = bottom to top
         * 4 = random
         */
        int slide_direction = 0;


        bool    initial          =  true;  // If this is the first call for the current transition
        GLfloat pos_last         =  0.0f;  // The last position for our image
        GLfloat pos_current      =  0.0f;  // The current position for our image
        int     pos_offset_ms    =  0;     // Used in combination with "fade_time_ms" to calculate the new position for the next frame
        int     random_direction =  0;     // If slide_direction = 4, this is where we store the random generated value

        GLfloat *tl;  // The images top left corner position
        GLfloat *tr;  // The images top right corner position
        GLfloat *bl;  // The images bottom left corner position
        GLfloat *br;  // The images bottom right corner position

        void slide(int direction, GLuint old_texture, GLuint new_texture);
        void left_to_right(GLuint old_texture, GLuint new_texture);
        void right_to_left(GLuint old_texture, GLuint new_texture);
        void top_to_bottom(GLuint old_texture, GLuint new_texture);
        void bottom_to_top(GLuint old_texture, GLuint new_texture);

    public:
        EFXSlide();
        ~EFXSlide();

        void configure(const char *key, void *value);
        bool render(GLuint old_texture, GLuint new_texture);
};
