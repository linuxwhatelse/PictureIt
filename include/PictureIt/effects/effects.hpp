#pragma once

#include <GL/gl.h>

enum class EFX {
    CROSSFADE,
    SLIDE,
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

        GLfloat *tex_tl;
        GLfloat *tex_tr;
        GLfloat *tex_bl;
        GLfloat *tex_br;

        int image_width  = 0;
        int image_height = 0;

        void img_mode_center();
        void img_mode_zoom();
        void img_mode_scale();

    public:
        /*!
         * @brief The mode that should be used to scale the image
         */
        MODE image_mode = MODE::ZOOM;

        /*!
         * @brief Width in pixel of the window rendering this visualization
         */
        int window_width = 0;

        /*!
         * @brief Height in pixel of the window rendering this visualization
         */
        int window_height = 0;

        /*!
         * @brief Width in pixel of the old image
         */
        int old_image_width = 0;

        /*!
         * @brief Height in pixel of the old image
         */
        int old_image_height = 0;

        /*!
         * @brief Width in pixel of the new image
         */
        int new_image_width = 0;

        /*!
         * @brief Height in pixel of the new image
         */
        int new_image_height = 0;

        EFXBase();
        ~EFXBase();

        /*!
         * @brief Configure a effect
         * @param key A, by the active effect, supported key
         * @param value The value for the previously defined :key:
         */
        virtual void configure(const char *key, void *value) = 0;

        /*!
         * @brief Renders the background image using the active effect till this methode returns :true:
         * @param old_texture Texture-ID representing the currently displayed image which is supposed to "go away"
         * @param new_texture Texture-ID representing the new image which is supposed to be "shown next"
         */
        virtual bool render(GLuint old_texture, GLuint new_texture) = 0;

        /*!
         * @brief Draw an OpenGL texture to the screen
         * @param texture_id OpenGL texture-id representing whatever should be drawn
         * @param is_new_image Wheter this is the new that will be displayed or the old one
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
        void draw_image(GLuint texture_id, bool is_new_image, GLfloat top_left[] = NULL, GLfloat top_right[] = NULL, GLfloat bottom_left[] = NULL, GLfloat bottom_right[] = NULL, float opacity = 1.0f);
};

// Include all available effects here
#include "crossfade.hpp"
#include "slide.hpp"
