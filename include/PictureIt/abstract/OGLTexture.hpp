#pragma once

#include "ITexture.hpp"

#include <GL/gl.h>


class OGLTexture : public ITexture {
    public:
        ~OGLTexture();

        GLuint texture_id;

        bool is_texture();
        bool load_image(const char *img_path);
        void render();
};
