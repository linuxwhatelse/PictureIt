#pragma once

#include "abstract/IRenderer.hpp"
#include "abstract/ITexture.hpp"
#include "abstract/ITransition.hpp"

#if defined(TARGET_LINUX)
    #include "abstract/OGLRenderer.hpp"
    #include "abstract/OGLTexture.hpp"
#endif


class PIFactory {
    public:
        static IRenderer *get_renderer() {
            #if defined(TARGET_LINUX)
                return new OGLRenderer();
            #endif
        };

        static ITexture *get_texture() {
            #if defined(TARGET_LINUX)
                return new OGLTexture();
            #endif
        };

        static ITransition *get_transition(ITransition::TRANSITION transition) {
            if (transition == ITransition::TRANSITION::CROSSFADE) {
                return new Crossfade();
            } else if (transition == ITransition::TRANSITION::SLIDE) {
                return new Slide();
            } else {
                return nullptr;
            }
        };
};
