#include "transitions/crossfade.hpp"

#include "utils.hpp"

#include <cstring>


void Crossfade::configure(const char *key, void *value) {
    if (strcmp(key, "fade_time_ms") == 0) {
        fade_time_ms = *(int*) value;
    }
}

bool Crossfade::update(ITexture *old_texture, ITexture *new_texture) {
    // Very first frame so we reset/initialize all variables properly
    if (this->initial) {
        this->initial = false;
        this->fade_current = 0.0;
        this->fade_offset_ms = PI_UTILS::get_time_in_ms() % this->fade_time_ms;
    }

    if (this->fade_offset_ms && this->fade_current < 1.0) {
        this->fade_current = (float) ((PI_UTILS::get_time_in_ms() -
            this->fade_offset_ms) % this->fade_time_ms ) / this->fade_time_ms;

        if (this->fade_current < this->fade_last) {
            this->fade_last       = 0.0;
            this->fade_current    = 1.0;
            this->fade_offset_ms  = 0;
        } else {
            this->fade_last = this->fade_current;
        }

        // Fade out old image
        if (old_texture != nullptr) {
            old_texture->alpha = (float)(1.0 - this->fade_current);
        }

        // Fade in new image
        if (new_texture != nullptr) {
            new_texture->alpha = (float)this->fade_current;
        }

        return false;
    }

    this->initial = true;

    return true;
}
