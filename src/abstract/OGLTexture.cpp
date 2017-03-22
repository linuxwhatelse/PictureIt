#include "abstract/OGLTexture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_BMP
#include "stb_image.h"


OGLTexture::~OGLTexture() {
    glDeleteTextures(1, &this->texture_id);
}


bool OGLTexture::is_texture() {
    return (bool) glIsTexture(this->texture_id);
}


bool OGLTexture::load_image(const char *img_path) {
    // Generate a new texture id if there isn't one
    if (!this->texture_id) {
        glGenTextures(1, &this->texture_id);
    }

    int x, y, n;
    unsigned char *data = stbi_load(img_path, &x, &y, &n, 0);

    this->width = x;
    this->height = y;

    if(data == nullptr) {
        return false;
    }

    glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, this->texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, data);
    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
    
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glDisable(GL_TEXTURE_2D);

    stbi_image_free(data);

    if (!this->is_texture()) {
        return false;
    }

    return true;
}

void OGLTexture::render() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, this->texture_id);

    glColor4f(this->red, this->green, this->blue, this->alpha);

    glBegin(GL_TRIANGLES);
        // Top Left
        glTexCoord2f(this->top_left[0], this->top_left[1]);
        glVertex2f(this->vert_top_left[0], this->vert_top_left[1]);

        // Top Right
        glTexCoord2f(this->top_right[0], this->top_right[1]);
        glVertex2f(this->vert_top_right[0], this->vert_top_right[1]);

        // Bottom Right
        glTexCoord2f(this->bottom_right[0], this->bottom_right[1]);
        glVertex2f(this->vert_bottom_right[0], this->vert_bottom_right[1]);
    glEnd();
    glBegin(GL_TRIANGLES);
        // Bottom Right
        glTexCoord2f(this->bottom_right[0], this->bottom_right[1]);
        glVertex2f(this->vert_bottom_right[0], this->vert_bottom_right[1]);

        // Bottom Left
        glTexCoord2f(this->bottom_left[0], this->bottom_left[1]);
        glVertex2f(this->vert_bottom_left[0], this->vert_bottom_left[1]);

        // Top Left
        glTexCoord2f(this->top_left[0], this->top_left[1]);
        glVertex2f(this->vert_top_left[0], this->vert_top_left[1]);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}
