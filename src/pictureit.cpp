#include "pictureit.hpp"
#include "utils.hpp"

#include <algorithm>


const char *PictureIt::image_filter[] = {
    ".jpg", ".png", ".jpeg", ".bmp",
    ".JPG", ".PNG", ".JPEG", ".BMP"
};

PictureIt::~PictureIt() {
    delete this->renderer;

    delete this->textures[0];
    delete this->textures[1];
}

bool PictureIt::set_transition(ITransition::TRANSITION t) {
    if (! this->transition_done)
        return false;

    delete this->transition;
    this->transition = PIFactory::get_transition(t);
    this->active_transition = t;

    delete this->textures[0];
    delete this->textures[1];

    this->textures[0] = PIFactory::get_texture();
    this->textures[1] = PIFactory::get_texture();

    //this->textures[0] = this->active_transition->get_texture();
    //this->textures[1] = this->active_transition->get_texture();

    this->set_display_mode(this->active_mode);

    return true;
};


bool PictureIt::set_display_mode(ITexture::MODE mode) {
    switch (mode) {
        default:
        case ITexture::MODE::STRETCH:
            this->textures[0]->stretch();
            this->textures[1]->stretch();
            break;
        case ITexture::MODE::CENTER:
            this->textures[0]->center();
            this->textures[1]->center();
            break;
        case ITexture::MODE::SCALE:
            this->textures[0]->scale();
            this->textures[1]->scale();
            break;
        case ITexture::MODE::ZOOM:
            this->textures[0]->zoom();
            this->textures[1]->zoom();
            break;
    }

    this->active_mode = mode;

    return true;
}

void PictureIt::set_window_size(int width, int height) {
    this->win_width = width;
    this->win_height = height;

    this->textures[0]->win_width  = width;
    this->textures[0]->win_height = height;

    this->textures[1]->win_width  = width;
    this->textures[1]->win_height = height;

    this->set_display_mode(this->active_mode);
}

const char* PictureIt::get_random_image() {
    int index = rand() % this->images.size();

    return this->images[index].c_str();
}


const char* PictureIt::get_next_image() {
    if (this->current_image_index < this->images.size()) {
        this->current_image_index++;
    } else {
        this->current_image_index = 0;
    }

    return this->images[this->current_image_index].c_str();
}


bool PictureIt::render() {
    this->renderer->prepare();

    if (! this->images.empty()) {
        // Check whether or not to update the current image
        if (this->img_update_by_interval && this->transition_done &&
            PI_UTILS::get_time_in_ms() >= (this->img_last_updated +
                                          (this->img_update_interval * 1000))) {
            this->img_update = true;
        }

        if (this->img_update == true) {
            this->img_last_updated = PI_UTILS::get_time_in_ms();
            this->transition_done  = false;
            this->img_update       = false;

            // Get a new image
            const char* img_path;
            if (this->img_pick_random) {
                img_path = get_random_image();
            } else {
                img_path = get_next_image();
            }

            if (this->textures[1]->load_image(img_path)) {
                // Set our window width/height and apply the
                // current display mode to our new texture
                this->textures[1]->win_height = this->win_width;
                this->textures[1]->win_width = this->win_height;
                this->set_display_mode(this->active_mode);
            } else {
                // Failed loading image, so when drawing the next frame we
                // immediately try to get a new one. If we'd do it in the
                // "load_image" methode we could end up in an endless-loop in
                // the main-thread if only broken images are available within
                // a the images vector.
                this->img_update = true;
            }
        }


        if (this->transition_done) {
            // From now on we keep drawing the current image ourself up
            // to the point where a new image will be displayed which will be
            // done by an effect again
            this->textures[0]->render();
        } else {
            if (this->textures[0]->is_texture()) {
                // Previous image, transition from previous to new
                this->transition_done = this->transition->update(
                    this->textures[0], this->textures[1]);
                this->set_display_mode(this->active_mode);
                this->textures[0]->render();
                this->textures[1]->render();
            } else {
                // No previous image, transition from black to new
                this->transition_done = this->transition->update(
                    nullptr, this->textures[1]);
                this->set_display_mode(this->active_mode);
                this->textures[1]->render();
            }

            // Effect finished, therefore we have to swap the position of both
            // textures
            if (this->transition_done) {
                swap(this->textures[0], this->textures[1]);

                this->textures[0]->render();
            }
        }
    }

    if (this->spectrum_enabled) {
        draw_spectrum();
    }

    this->renderer->finish();

    return this->transition_done;
}


void PictureIt::update_image(bool force_update) {
    if (force_update) {
        this->img_update = true;
    } else {
        if (this->transition_done) {
            this->img_update = true;
        }
    }
}


void PictureIt::load_images(const char *image_root_dir) {
    current_image_index = -1;
    images.clear();

    PI_UTILS::list_dir(image_root_dir, this->images, true, true,
        this->image_filter, sizeof(this->image_filter));

    sort(this->images.begin(), this->images.end());
}
