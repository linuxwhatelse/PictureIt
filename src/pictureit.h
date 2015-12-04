#pragma once

#include "spectrum.h"
#include "effects.h"

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
        GLuint img_texture_ids[2]  =  {};
        bool   img_update          =  true;
        int    img_current_index   =  -1;
        time_t img_last_updated    =  time(0);
        bool   img_effect_finished =  true;
        EFXS   img_transition_efx  =  EFXS::CROSSFADE;

        vector<string> images;

        const char *image_filter[3] = { "*.jpg", "*.png", "*.jpeg" };

        void start_render();
        void finish_render();
        const char* get_random_image();
        const char* get_next_image();

    public:
        EFXBase  *EFX      = NULL;

        // Values that can be configured by whoever implements PictureIt
        // Note that the :Spectrum: class (which we inherit from) exposes configurable values
        // all prefixed with "spectrum_*"
        bool   img_pick_random         = true;
        bool   img_update_by_interval  = true;
        int    img_update_interval     = 180;

        bool   spectrum_enabled        = true;

        
        PictureIt(int spectrum_bar_count = 64): Spectrum(spectrum_bar_count) {
            this->EFX = new EFXCrossfade();
            glGenTextures(2, img_texture_ids);
        };

        ~PictureIt();

        /*!
         * @brief If NO transition is active, sets the new transition effect
         * @param efx Effect to activate. Available effects are defined in the effects.h file within the :EFXS: enum
         * @return true if the new effect was activated, false otherwise
         */
        bool set_img_transition_efx(EFXS efx);

        /*!
         * @brief Get the current image transition effect
         * @return the current image transition effect
         */
        EFXS get_img_transition_efx();

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
