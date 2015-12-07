#pragma once

#include <GL/gl.h>

enum class EFX {
    CROSSFADE,
    SLIDE,
    // FLIP
};

enum class MODE {
    STRETCH,
    CENTER,
    SCALE,
    ZOOM,
};

class EFXBase {
    private:
        GLfloat *tl;
        GLfloat *tr;
        GLfloat *bl;
        GLfloat *br;

        void img_mode_center();
        void img_mode_zoom();
        void img_mode_scale();

    public:
        MODE image_mode   = MODE::ZOOM;

        int window_width  = 0;
        int window_height = 0;
        int image_width   = 0;
        int image_height  = 0;

        EFXBase();
        ~EFXBase();

        virtual void configure(const char *key, void *value) = 0;
        virtual bool render(GLuint old_texture, GLuint new_texture) = 0;

        /*!
         * @brief Returns the current time in milliseconds
         * @return current time in milliseconds
         */
        long int get_current_time_ms();

        /*!
         * @brief Draw an OpenGL texture to the screen
         * @param texture_id OpenGL texture-id representing whatever should be drawn
         * @param tl array where tl[0] = x-pos and tl[1] = y-pos of the textures "TOP LEFT" corner
         * @param tr array where tr[0] = x-pos and tr[1] = y-pos of the textures "TOP RIGHT" corner
         * @param bl array where bl[0] = x-pos and bl[1] = y-pos of the textures "BOTTOM LEFT" corner
         * @param br array where br[0] = x-pos and br[1] = y-pos of the textures "BOTTOM RIGHT" corner
         * @param opacity Opacity of the texture ranging from 0.0f to 1.0f
         *
         * Coordinate-System:
         *
         *       y
         *  -1.0 |
         *       |
         *   0.0 |
         *       |
         *   1.0 |_____|_____ x
         *     -1.0   0.0   1.0
         *
         *  TOP LEFT     : x = -1.0f | y = -1.0f
         *  TOP RIGHT    : x =  1.0f | y = -1.0f
         *  BOTTOM LEFT  : x = -1.0f | y =  1.0f
         *  BOTTOM RIGHT : x =  1.0f | y =  1.0f
         */
        void draw_image(GLuint texture_id, GLfloat top_left[] = NULL, GLfloat top_right[] = NULL, GLfloat bottom_left[] = NULL, GLfloat bottom_right[] = NULL, float opacity = 1.0f);
};

// Include all available effects here
#include "crossfade.h"
#include "slide.h"
#include "flip.h"
