#include "abstract/ITexture.hpp"

void ITexture::stretch() {
    this->top_left[0] = 0.0;
    this->top_right[0] = 1.0;
    this->bottom_left[0] = 0.0;
    this->bottom_right[0] = 1.0;

    this->top_left[1] = 0.0;
    this->top_right[1] = 0.0;
    this->bottom_left[1] = 1.0;
    this->bottom_right[1] = 1.0;


    this->vert_top_left[0] = -1.0;
    this->vert_top_right[0] = 1.0;
    this->vert_bottom_left[0] = -1.0;
    this->vert_bottom_right[0] = 1.0;

    this->vert_top_left[1] = -1.0;
    this->vert_top_right[1] = -1.0;
    this->vert_bottom_left[1] = 1.0;
    this->vert_bottom_right[1] = 1.0;
}


void ITexture::center() {
    // Reset to base values
    this->stretch();

    if (this->win_width > this->width) {
        float diff_width  = 1.0 - ((float) this->width /
                                   (float) this->win_width);

        this->top_left[0] = this->top_left[0] + diff_width;
        this->top_right[0] = this->top_right[0] - diff_width;
        this->bottom_left[0] = this->bottom_left[0] + diff_width;
        this->bottom_right[0] = this->bottom_right[0] - diff_width;
    } else {
        float overflow = (float)this->width - (float)this->win_width;
        float diff = (overflow / this->width) / 2;

        this->vert_top_left[0] = this->vert_top_left[0] + diff;
        this->vert_top_right[0] = this->vert_top_right[0] - diff;
        this->vert_bottom_left[0] = this->vert_bottom_left[0] + diff;
        this->vert_bottom_right[0] = this->vert_bottom_right[0] - diff;
    }

    if ( this->win_height > this->height ) {
        float diff_height = 1.0 - ((float) this->height /
                                   (float) this->win_height);

        this->top_left[1] = this->top_left[1] + diff_height;
        this->top_right[1] = this->top_right[1] + diff_height;
        this->bottom_left[1] = this->bottom_left[1] - diff_height;
        this->bottom_right[1] = this->bottom_right[1] - diff_height;
    } else {
        float overflow = (float)this->height - (float)this->win_height;
        float diff = (overflow / this->height) / 2;

        this->vert_top_left[1] = this->vert_top_left[1] + diff;
        this->vert_top_right[1] = this->vert_top_right[1] + diff;
        this->vert_bottom_left[1] = this->vert_bottom_left[1] - diff;
        this->vert_bottom_right[1] = this->vert_bottom_right[1] - diff;
    }
}


void ITexture::scale() {
    // Reset to base values
    this->stretch();

    float ratio_img = (float)this->width / (float)this->height;
    float ratio_win = (float)this->win_width / (float)this->win_height;

    if (ratio_win >= ratio_img) {
        int scaled_image_width = (this->width * ((float)this->win_height /
                                                 (float)this->height));

        float diff_width = 1.0 - ((float)scaled_image_width /
                                  (float)this->win_width);

        this->top_left[0] = this->top_left[0] + diff_width;
        this->top_right[0] = this->top_right[0] - diff_width;
        this->bottom_left[0] = this->bottom_left[0] + diff_width;
        this->bottom_right[0] = this->bottom_right[0] - diff_width;
    } else {
        float diff = (ratio_img - ratio_win) / (2 * ratio_img);

        this->vert_top_left[0] = this->vert_top_left[0] + diff;
        this->vert_top_right[0] = this->vert_top_right[0] - diff;
        this->vert_bottom_left[0] = this->vert_bottom_left[0] + diff;
        this->vert_bottom_right[0] = this->vert_bottom_right[0] - diff;
    }
}


void ITexture::zoom() {
    // Reset to base values
    this->stretch();

    float ratio_img = (float)this->height / (float)this->width;
    float ratio_win = (float)this->win_height / (float)this->win_width;

    if (ratio_win >= ratio_img) {
        float zoomed_image_height = (this->height * ((float)this->win_width /
                                                     (float)this->width));

        float diff_height  = 1.0 - ((float)zoomed_image_height /
                                    (float)this->win_height);

        this->top_left[1] = this->top_left[1] + diff_height;
        this->top_right[1] = this->top_right[1] + diff_height;
        this->bottom_left[1] = this->bottom_left[1] - diff_height;
        this->bottom_right[1] = this->bottom_right[1] - diff_height;
    } else {
        float diff = (ratio_img - ratio_win) / (2 * ratio_img);

        this->vert_top_left[1] = this->vert_top_left[1] + diff;
        this->vert_top_right[1] = this->vert_top_right[1] + diff;
        this->vert_bottom_left[1] = this->vert_bottom_left[1] - diff;
        this->vert_bottom_right[1] = this->vert_bottom_right[1] - diff;
    }
}
