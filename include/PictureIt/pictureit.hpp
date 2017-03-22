#pragma once

#include "utils.hpp"
#include "spectrum.hpp"

#include "Factory.hpp"
#include "abstract/IRenderer.hpp"

#include <string>
#include <vector>


class PictureIt : public Spectrum {
    private:
        /*!
         * @brief Filter for image types
         */
        static const char *image_filter[];

        /*!
         * @brief Textures to be drawn
         *        0: The currently displayed texture.
         *        1: The next texture
        */
        ITexture *textures[2];

        /*!
         * @biref The windows current width. See `set_window_size`.
         */
        int win_width = 0;
        /*!
         * @biref The windows current height. See `set_window_size`.
         */
        int win_height = 0;

        /*!
         * @brief Whether or not to update to the next image.
         */
        bool img_update = true;
        /*!
         * @brief Time in ms when the image was last updated.
         */
        long img_last_updated = PI_UTILS::get_time_in_ms();

        /*!
         * @brief List of image-paths to be displayed.
         */
        vector<string> images;

        /*!
         * @brief Index of the currently displayed image. See `images` and 'get_next_image()'.
         */
        int current_image_index = -1;

        /*!
         * @brief Whether or not a transition is in progress.
         */
        bool transition_done = true;

        /*!
         * @brief The currently active transition.
         */
        ITransition::TRANSITION active_transition = ITransition::TRANSITION::CROSSFADE;

        /*!
         * @brief The currently active display mode.
         */
        ITexture::MODE active_mode = ITexture::MODE::STRETCH;


        /*!
         * @brief A abstract renderer instance to prepare, finish, ... frames.
         */
        IRenderer *renderer = nullptr;

        /*!
         * @brief Pick a random image to be displayed. See 'images'.
         * @returns An absolute path to an images.
         */
        const char* get_random_image();
        /*!
         * @brief Pick the next image to be displayed. See `images`.
         * @returns An absolute path to an images.
         */
        const char* get_next_image();

    public:
        /*!
         * @brief A abstract instance of an active transition which can be used
                  to configure said transition via its `configure` method.
         */
        ITransition *transition = nullptr;

        /*!
         * @brief If a random images should be picked every time the image updates
         * If set to :false: the images will be displayed in alphabetical order
         */
        bool img_pick_random = true;

        /*!
         * @brief Whether the images should be updated by interval or not
         */
        bool img_update_by_interval = true;

        /*!
         * @brief Image update interval in seconds
         */
        int img_update_interval = 180;

        /*!
         * @brief Whether the spectrum should be enabled or not
         */
        bool spectrum_enabled = true;

        /*!
         * @brief Create a new instance of PictureIt
         * @param spectrum_bar_count Amount of bars the spectrum should have. This excludes the mirrored parts
         */
        PictureIt(int spectrum_bar_count = 64): Spectrum(spectrum_bar_count) {
            this->renderer = PIFactory::get_renderer();
            this->renderer->init();

            this->textures[0] = PIFactory::get_texture();
            this->textures[1] = PIFactory::get_texture();

            this->set_transition(this->active_transition);
        };

        /*!
         * @brief PictureIt destructor.
         */
        ~PictureIt();

        /*!
         * @brief PictureIt needs to know about the windows width and height
         *        to properly scale images.
         *        Whenever the windows size changes, call this method with
         *        the windows new width and height.
         * @param width The windows new width value
         * @param height The windows new height value
         */
        void set_window_size(int width, int height);

        /*!
         * @brief If NO transition is active, sets the new transition effect
         * @param efx Effect to activate. Available effects are defined in the effects.h file within the :EFX: enum
         * @return true if the new effect was activated, false otherwise
         */
        bool set_transition(ITransition::TRANSITION t);

        /*!
         * @brief If NO transition is active, sets the new transition effect
         * @param mode Display mode to set. Available modes are defined in
            the effects.h file within the :EFX: enum
         * @return true if the new effect was activated, false otherwise
         */
        bool set_display_mode(ITexture::MODE mode);

        /*!
         * @brief Get the current image transition effect
         * @return the current image transition effect
         */
        ITransition::TRANSITION get_transition() {
            return this->active_transition;
        };

        /*!
         * @brief Get the current image display mode
         * @return the current image display mode
         */
        ITexture::MODE get_display_mode() {
            return this->active_mode;
        };

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
