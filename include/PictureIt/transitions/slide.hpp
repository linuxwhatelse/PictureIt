#pragma once

#include "abstract/ITexture.hpp"
#include "abstract/ITransition.hpp"


class Slide: public ITransition {
    public:
        enum class DIRECTION {
            LEFT_TO_RIGHT,
            RIGHT_TO_LEFT,
            TOP_TO_BOTTOM,
            BOTTOM_TO_TOP,
            RANDOM,
        };

        //~Slide();

        void configure(const char *key, void *value);
        bool update(ITexture *old_texture, ITexture *new_texture);

    private:
        /*!
         * @brief Time in milliseconds the fade effect should take
        */
        int slide_time_ms = 1000;

        /*!
         * @brief Direction configured by the user
         */
        DIRECTION direction = DIRECTION::RIGHT_TO_LEFT;

        /*!
         * @brief Either direction or, if direction is RANDOM
         * a randomly picked direction.
         */
        DIRECTION _direction = this->direction;

        bool  initial       =  true;  // If this is the first call for the current transition
        float pos_last      =  0.0;   // The last position for our image
        float pos_current   =  0.0;   // The current position for our image
        int   pos_offset_ms =  0;     // Used in combination with "fade_time_ms" to calculate the new position for the next frame

        float tl[2];  // The images top left corner position
        float tr[2];  // The images top right corner position
        float bl[2];  // The images bottom left corner position
        float br[2];  // The images bottom right corner position

        void left_to_right(ITexture *old_texture, ITexture *new_texture);
        void right_to_left(ITexture *old_texture, ITexture *new_texture);
        void top_to_bottom(ITexture *old_texture, ITexture *new_texture);
        void bottom_to_top(ITexture *old_texture, ITexture *new_texture);
};
