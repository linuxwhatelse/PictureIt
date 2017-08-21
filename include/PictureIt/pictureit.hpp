#pragma once

#include <string>
#include <vector>
#include <memory>

#include "utils.hpp"
#include "config.hpp"

#include "buffer.hpp"
#include "fft.hpp"
#include "texture/texture.hpp"

#include "spectrum/spectrum.hpp"

using namespace PI;


class PictureIt {
private:
    typedef std::unique_ptr<Spectrum> spec_ptr;
    typedef std::shared_ptr<Texture> tex_ptr;

    static const char *image_filter[];

    Config::PictureIt cfg;

    int win_width = 0;
    int win_height = 0;

    bool img_update = true;
    long img_last_updated = Utils::get_time_in_ms();

    int current_image_index = -1;

    std::vector<string> images;

    tex_ptr background[2] = {nullptr, nullptr};
    Config::Texture bg_cfg;

    std::vector<tex_ptr> textures;
    std::vector<spec_ptr> spectra;

    void load_images();
    string get_new_image();


public:
    PictureIt(Config::PictureIt cfg);
    // disable copy construction
    PictureIt(const PictureIt&) = delete;
    ~PictureIt();

    /**
     * Updates the viewport with the given width and height.
     * @param width The windows width in pixel.
     * @param height The windows height in pixel.
     */
    void set_window_size(int width, int height);

    /**
     * Adds a new spectrum to the render queue.
     * @param cfg Configuration used for the new spectrum.
     * @returns an id associated with this spectrum.
     */
    const unsigned int add_spectrum(const Config::Spectrum& cfg);

    /**
     * Removes a spectrum from the render queue.
     * @param id The id returned by add_spectrum.
     * @return true if a valid id was given and the spectrum was removed,
               false otherwise
     */
    bool remove_spectrum(unsigned int id);


    /**
     * Adds a new texture to the render queue.
     * @param cfg Configuration used for the new texture.
     * @returns an id associated with this texture.
     */
    //const unsigned int add_texture(const Config::Texture& cfg);
    tex_ptr add_texture(const Config::Texture& cfg);

    /**
     * Removes a texture from the render queue.
     * @param id The id returned by add_texture.
     * @param force If true, the out-transition will be ignored and the texture
              will be removed immediately.
     * @return true if a valid id was given and the texture will be removed,
               false otherwise.
     */
    bool remove_texture(unsigned int id, bool force);

    /**
     * Transition to the next image.
     * @return true if the transition will be triggered, false otherwise e.g.
               due to there being a active transition.
     */
    bool next_image();

    /**
     * Render loop for PictureIt. Call this method in your main-loop.
     * @param fft FFT data to be used for the spectra
     */
    void draw(FFT& fft);
    void draw();
};
