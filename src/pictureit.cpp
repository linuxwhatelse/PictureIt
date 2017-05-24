#include "pictureit.hpp"
#include "utils.hpp"

#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <thread>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


const char *PictureIt::image_filter[] = {
    ".jpg", ".png", ".jpeg", ".bmp",
    ".JPG", ".PNG", ".JPEG", ".BMP"
};

const std::chrono::time_point<std::chrono::high_resolution_clock> t_start =
    std::chrono::high_resolution_clock::now();


PictureIt::PictureIt(Config::PictureIt pi_cfg) : cfg(pi_cfg) {
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(1);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    this->bg_cfg.mode = pi_cfg.display_mode;
    this->bg_cfg.transition_in = pi_cfg.transition;
    this->bg_cfg.transition_out = pi_cfg.transition;

    this->load_images();
};

PictureIt::~PictureIt() {}

const unsigned int PictureIt::add_spectrum(const Config::Spectrum& cfg) {
    const unsigned int id = this->spectra.size() + 1;

    this->spectra.push_back(make_unique<Spectrum>(cfg, id));
    return id;
}

bool PictureIt::remove_spectrum(unsigned int id) {
    for(unsigned i = 0; i < this->spectra.size(); i++) {
        if (this->spectra.at(i)->get_id() == id) {
            this->spectra.erase(this->spectra.begin() + i);
            return true;
        }
    }

    return false;
}

//const unsigned int PictureIt::add_texture(const Config::Texture& cfg) {
PictureIt::tex_ptr PictureIt::add_texture(const Config::Texture& cfg) {
    // As the id directly relates to the texture unit, we have to add our fixed
    // background textures to the dynamic size of textures
    const unsigned int id = (this->textures.size() + (sizeof(this->background) /
                             sizeof(*this->background)) + 1);

    tex_ptr tex = make_shared<Texture>(cfg, id);
    this->textures.push_back(move(tex));

    return tex;
}

bool PictureIt::remove_texture(unsigned int id, bool force) {
    for(unsigned i = 0; i < this->textures.size(); i++) {
        if (this->textures.at(i)->get_id() == id) {
            if (force) {
                this->textures.erase(this->textures.begin() + i);
            } else {
                this->textures.at(i)->hide();
            }

            return true;
        }
    }

    return false;
}

void PictureIt::set_window_size(int width, int height) {
    this->win_width = width;
    this->win_height = height;

    glViewport(0, 0, width, height);
}

string PictureIt::get_new_image() {
    if (this->cfg.pick_random) {
        int size = this->images.size();
        this->current_image_index = Utils::get_random(0, size - 1);
    } else {
        if (this->current_image_index < this->images.size()) {
            this->current_image_index++;
        } else {
            this->current_image_index = 0;
        }
    }

    return this->images[this->current_image_index];
}

void PictureIt::draw(FFT& fft) {
    std::chrono::time_point<std::chrono::steady_clock> t_fps = (
        std::chrono::steady_clock::now() +
            std::chrono::microseconds(1000000 / this->cfg.max_fps - 100));

    glClearColor(this->cfg.bg_color[0], this->cfg.bg_color[1],
                 this->cfg.bg_color[2], this->cfg.bg_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Handle background image
    if (! this->images.empty()) {
        // Check whether or not to update the current image
        if (this->cfg.update_by_interval &&
            Utils::get_time_in_ms() >= (this->img_last_updated +
                                        (this->cfg.update_interval * 1000))) {
            this->img_update = true;
        }

        if (this->img_update == true) {
            this->img_update = false;
            this->img_last_updated = Utils::get_time_in_ms();

            if (this->background[0] == nullptr) {
                // Setup / First time we do something
                this->bg_cfg.path = get_new_image();
                this->background[0] = make_unique<Texture>(this->bg_cfg, 0);
            } else {
                // Subsequential runs where we update and transition between
                // two images
                this->background[0]->hide();

                this->bg_cfg.path = get_new_image();
                if (this->background[1] == nullptr) {
                    this->background[1] = make_unique<Texture>(this->bg_cfg, 1);
                } else {
                    this->background[1]->configure(this->bg_cfg);
                }
            }
        }

        if (this->background[0]->is_hidden()) {
            swap(this->background[0], this->background[1]);
        }

        // Draw background images
        for (tex_ptr& texture: this->background) {
            if (texture != nullptr && texture->is_valid()
                    && ! texture->is_hidden())
                texture->draw();
        }
    }


    // Draw User defined textures
    for(unsigned i = 0; i < this->textures.size(); i++) {
        this->textures.at(i);
        if (! this->textures.at(i)->is_valid())
            continue;

        if (this->textures.at(i)->is_hidden()) {
            this->textures.erase(this->textures.begin() + i);
            continue;
        }

        this->textures.at(i)->draw();
    }


    // Draw user defined spectra
    for (spec_ptr& spectrum: this->spectra) {
        spectrum->update_fft(fft);
        spectrum->draw();
    }

    // Limit fps
    std::this_thread::sleep_until(t_fps);
}

bool PictureIt::next_image() {
    if (this->background[0] != nullptr &&
            this->background[0]->is_transitioning())
        return false;

    this->img_update = true;
}

void PictureIt::load_images() {
    current_image_index = -1;
    images.clear();

    Utils::list_dir(this->cfg.image_dir, this->images, this->cfg.recursive, true,
        this->image_filter, sizeof(this->image_filter));

    sort(this->images.begin(), this->images.end());
}
