#pragma once

#include "ITexture.hpp"

#include <memory>

class ITransition {
    public:
        enum TRANSITION {
            CROSSFADE,
            SLIDE
        };

        std::unique_ptr<int> p;
        ITransition() {
            p = std::make_unique<int>(1);
        };
        virtual ~ITransition() = default;

        /*!
         * @brief Configure a transition
         * @param key A, by the active transition, supported key
         * @param value The value for the previously defined :key:
         */
        virtual void configure(const char *key, void *value) = 0;

        /*!
         * @brief Creates and returns a new texture which will be passed to
         *        the transitions :update: method later on.
         *        A Texture can either be retrieved via :PIFactory::get_texture:
         *        or a special texture instance created by the transition itself.
         * @returns A new texture based on the ITexture interface.
         */
        //virtual ITexture get_texture() = 0;

        /*!
         * @brief Update the properties of both textures the create a transition
         * @param old_texture Texture representing the currently displayed image which is supposed to "go away"
         * @param new_texture Texture representing the new image which is supposed to be "shown next"
         */
        virtual bool update(ITexture *old_texture, ITexture *new_texture) = 0;
};

#include "transitions/crossfade.hpp"
#include "transitions/slide.hpp"
