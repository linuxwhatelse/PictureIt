#pragma once

#include <GL/gl.h>

enum class EFXS {
    CROSSFADE,
    SLIDE
};

class EFXBase {
    public:
        virtual void configure(const char *key, int value) = 0;
        virtual bool render(GLuint old_texture, GLuint new_texture) = 0;
};

// Include all available effects here
#include "crossfade.h"
#include "slide.h"
