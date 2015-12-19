#include "crossfade.h"
#include "utils.h"

#include <cstring>

void EFXCrossfade::configure(const char *key, void *value) {
    if ( strcmp( key, "fade_time_ms") == 0 )
        fade_time_ms = *(int*) value;
}

bool EFXCrossfade::render(GLuint old_texture, GLuint new_texture) {
    // Very first frame so we reset/initialize all variables properly
    if ( this->initial ) {
        this->initial = false;
        this->fade_current = 0.0f;
        this->fade_offset_ms = get_current_time_ms() % this->fade_time_ms;
    }

    if ( this->fade_offset_ms && this->fade_current < 1.0f ) {
        this->fade_current = (float) ( ( get_current_time_ms() - this->fade_offset_ms ) % this->fade_time_ms ) / this->fade_time_ms;

        if ( this->fade_current < this->fade_last ) {
            this->fade_last       = 0.0f;
            this->fade_current    = 1.0f;
            this->fade_offset_ms  = 0;
        } else
            this->fade_last = this->fade_current;

        // Fade out old image
        draw_image( old_texture, false, NULL, NULL, NULL, NULL, 1.0f - this->fade_current );

        // Fade in new image
        draw_image( new_texture, true, NULL, NULL, NULL, NULL, this->fade_current );

        return false;
    }

    this->initial = true;

    return true;
}