#pragma once

#include <GL/gl.h>
#include "renderer/renderer.hpp"

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

enum class DISPLAYMODE {
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
        virtual void configure(const char *key, void *value) {};

        /*!
         * @brief Renders the background image using the active effect till this methode returns :true:
         * @param old_texture Texture representing the currently displayed image which is supposed to "go away"
         * @param new_texture Texture representing the new image which is supposed to be "shown next"
         */
        virtual bool render(const Renderer &renderer, Texture &old_texture, Texture &new_texture) {};

        /*!
         * @brief Returns the current time in milliseconds
         * @return current time in milliseconds
         */
        long int get_current_time_ms();
};

// Include all available effects here
#include "crossfade.hpp"
#include "slide.hpp"
