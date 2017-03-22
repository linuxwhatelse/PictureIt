#include "transitions/slide.hpp"
#include "utils.hpp"

#include <cstring>


void Slide::configure(const char *key, void *value) {
    if (strcmp(key, "slide_time_ms") == 0) {
        this->slide_time_ms = *(int*) value;

    } else if (strcmp(key, "slide_direction") == 0) {
        this->direction = *(DIRECTION*) value;
    }
}

bool Slide::update(ITexture *old_texture, ITexture *new_texture) {
    // Very first frame so we reset/initialize all variables properly
    if (this->initial) {
        this->initial = false;
        this->pos_current = 0.0;
        this->pos_offset_ms = (PI_UTILS::get_time_in_ms() % this->slide_time_ms);
        if (this->direction == DIRECTION::RANDOM) {
            printf("random\n");
            // Random pick one of the first THREE directions (4th would be
            // be RANDOM again)
            this->_direction = (DIRECTION) (rand() % 4);
        } else {
            this->_direction = this->direction;
        }
    }

    if (this->pos_offset_ms && this->pos_current < 2.0) {
        this->pos_current = (float)((PI_UTILS::get_time_in_ms() - this->pos_offset_ms) %
                                     this->slide_time_ms) / this->slide_time_ms * 2;

        if (this->pos_current < this->pos_last) {
            this->pos_last       = 0.0;
            this->pos_current    = 2.0;
            this->pos_offset_ms  = 0;
        } else {
            this->pos_last = this->pos_current;
        }

        switch (this->_direction) {
            case DIRECTION::LEFT_TO_RIGHT:
                left_to_right(old_texture, new_texture);
                break;
            case DIRECTION::RIGHT_TO_LEFT:
                right_to_left(old_texture, new_texture);
                break;
            case DIRECTION::TOP_TO_BOTTOM:
                top_to_bottom(old_texture, new_texture);
                break;
            case DIRECTION::BOTTOM_TO_TOP:
                bottom_to_top(old_texture, new_texture);
                break;
        }

        return false;
    }

    this->initial = true;

    return true;
}

void Slide::left_to_right(ITexture *old_texture, ITexture *new_texture) {
    // Slide out old image
    if (old_texture != nullptr) {
        old_texture->vert_top_left[0] = -1.0 + this->pos_current;
        old_texture->vert_top_right[0] = 1.0 + this->pos_current;
        old_texture->vert_bottom_left[0] = -1.0 + this->pos_current;
        old_texture->vert_bottom_right[0] = 1.0 + this->pos_current;
        old_texture->render();
    }

    // Slide in new image
    if (new_texture != nullptr) {
        new_texture->vert_top_left[0] = -3.0 + this->pos_current;
        new_texture->vert_top_right[0] = -1.0 + this->pos_current;
        new_texture->vert_bottom_left[0] = -3.0 + this->pos_current;
        new_texture->vert_bottom_right[0] = -1.0 + this->pos_current;
        new_texture->render();
    }
};

void Slide::right_to_left(ITexture *old_texture, ITexture *new_texture) {
    // Slide out old image
    if (old_texture != nullptr) {
        old_texture->vert_top_left[0] = -1.0 - this->pos_current;
        old_texture->vert_top_right[0] = 1.0 - this->pos_current;
        old_texture->vert_bottom_left[0] = -1.0 - this->pos_current;
        old_texture->vert_bottom_right[0] = 1.0 - this->pos_current;
        old_texture->render();
    }

    // Slide in new image
    if (new_texture != nullptr) {
        new_texture->vert_top_left[0] = 1.0 - this->pos_current;
        new_texture->vert_top_right[0] = 3.0 - this->pos_current;
        new_texture->vert_bottom_left[0] = 1.0 - this->pos_current;
        new_texture->vert_bottom_right[0] = 3.0 - this->pos_current;
        new_texture->render();
    }
};

void Slide::top_to_bottom(ITexture *old_texture, ITexture *new_texture) {
    // Slide out old image
    if (old_texture != nullptr) {
        old_texture->vert_top_left[1] = -1.0 + this->pos_current;
        old_texture->vert_top_right[1] = -1.0 + this->pos_current;
        old_texture->vert_bottom_left[1] = 1.0 + this->pos_current;
        old_texture->vert_bottom_right[1] = 1.0 + this->pos_current;
        old_texture->render();
    }

    // Slide in new image
    if (new_texture != nullptr) {
        new_texture->vert_top_left[1] = -3.0 + this->pos_current;
        new_texture->vert_top_right[1] = -3.0 + this->pos_current;
        new_texture->vert_bottom_left[1] = -1.0 + this->pos_current;
        new_texture->vert_bottom_right[1] = -1.0 + this->pos_current;
        new_texture->render();
    }
};

void Slide::bottom_to_top(ITexture *old_texture, ITexture *new_texture) {
    // Slide out old image
    if (old_texture != nullptr) {
        old_texture->vert_top_left[1] = -1.0 - this->pos_current;
        old_texture->vert_top_right[1] = -1.0 - this->pos_current;
        old_texture->vert_bottom_left[1] = 1.0 - this->pos_current;
        old_texture->vert_bottom_right[1] = 1.0 - this->pos_current;
        old_texture->render();
    }

    // Slide in new image
    if (new_texture != nullptr) {
        new_texture->vert_top_left[1] = 1.0 - this->pos_current;
        new_texture->vert_top_right[1] = 1.0 - this->pos_current;
        new_texture->vert_bottom_left[1] = 3.0 - this->pos_current;
        new_texture->vert_bottom_right[1] = 3.0 - this->pos_current;
        new_texture->render();
    }
};
