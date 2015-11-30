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
        // all prefixed with "spectrum_"
        EFFECT img_efx                 = EFFECT::Crossfade;
        bool   img_pick_random         = true;
        bool   img_update_by_interval  = true;
        int    img_update_interval     = 180;

        bool   spectrum_enabled        = true;

        PictureIt(int spectrum_bar_count = 64): Spectrum(spectrum_bar_count){};
        ~PictureIt();

        void init();

        void update_image(bool force_update = false);

        void load_images(const char *image_root_dir);
        
        bool render();
};
