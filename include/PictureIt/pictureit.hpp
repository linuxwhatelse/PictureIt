#pragma once

#include "spectrum.hpp"
#include "effects/effects.hpp"

#include <string>
#include <vector>
#include <sys/time.h>

using namespace std;

class PictureIt : public Spectrum {
    private:
        /*
        :img_texture_ids: holds the texture-ids for images:
           0: The current displayed image.
           1: The next image which fades in.
        */
        GLuint  img_texture_ids[2]  =  {};
        bool    img_update          =  true;
        int     img_current_index   =  -1;
        time_t  img_last_updated    =  time(0);
        bool    img_effect_finished =  true;
        EFX     img_transition_efx  =  EFX::CROSSFADE;

        int image_width            =  0;
        int image_height           =  0;

        vector<string> images;

        const char *image_filter[3] = { "*.jpg", "*.png", "*.jpeg" };

        void start_render();
        void finish_render();
        const char* get_random_image();
        const char* get_next_image();

    public:
        /*!
         * @brief Pointer to the currently active effect used to transition between images
         * See :set_img_transition_efx: on how to change it
         */
        EFXBase *efx = NULL;

        /*!
         * @brief Width in pixel of the window rendering this visualization
         */
        int window_width = 0;

        /*!
         * @brief Height in pixel of the window rendering this visualization
         */
        int window_height = 0;

        /*!
         * @brief If a random images should be picked everytime the image updates
         * If set to :false: the images will be displayed in alphabetical order
         */
        bool img_pick_random = true;

        /*!
         * @brief Whether the images should be updated by intervall or not
         */
        bool img_update_by_interval = true;

        /*!
         * @brief Image update interval in seconds
         */
        int img_update_interval = 180;

        /*!
         * @brief Whether the spectrum should be enabled or not
         */
        bool spectrum_enabled = true;

        /*!
         * @brief Create a new instance of PictureIt
         * @param spectrum_bar_count Amount of bars the spectrum should have. This excludes the mirrored parts
         */
        PictureIt(int spectrum_bar_count = 64): Spectrum(spectrum_bar_count) {
            set_img_transition_efx(this->img_transition_efx);

            glGenTextures(2, this->img_texture_ids);
        };

        ~PictureIt();

        /*!
         * @brief If NO transition is active, sets the new transition effect
         * @param efx Effect to activate. Available effects are defined in the effects.h file within the :EFX: enum
         * @return true if the new effect was activated, false otherwise
         */
        bool set_img_transition_efx(EFX efx);

        /*!
         * @brief Get the current image transition effect
         * @return the current image transition effect
         */
        EFX get_img_transition_efx();

        /*!
         * @brief Display the next image
         * @param force_update If a transition if active, the image will not be updated unless this param is set to true
         */
        void update_image(bool force_update = false);

        /*!
         * @brief Loads all found images into the stack of images to display
         * @param img_root_dir absolute path to a directory which should be search recursively for images
         */
        void load_images(const char *image_root_dir);

        /*!
         * @brief Renders one frame
         * @return true if a image transition effect is active, false otherwise
         */
        bool render();
};
