#pragma once

#include "abstract/ITexture.hpp"
#include "abstract/ITransition.hpp"


class Crossfade: public ITransition {
    public:
        //~Crossfade();

        void configure(const char *key, void *value);
        bool update(ITexture *old_texture, ITexture *new_texture);

    private:
        // CONFIG
        int   fade_time_ms    =    1000;  // Time in milliseconds this effect has to take


        bool  initial         =    true;  // If this is the first call for the current transition
        float fade_last       =    0.0f;  // The last alpha value of our image
        float fade_current    =    0.0f;  // The current alpha value of our image
        int   fade_offset_ms  =    0;     // Used in combination with "fade_time_ms" to calculate the new alpha value for the next frame
};
