#pragma once

#include "gl_lite.hpp"

#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "gl_utils.hpp"
#include "program.hpp"
#include "config.hpp"

#include "transitions/itransition.hpp"
#include "transitions/factory.hpp"

using namespace PI;


class Texture {
private:
    int tex_unit;
    bool initial = true;

    GL::Texture texture;

    GL::Program sh;
    GL::VAO vao;
    GL::Buffer vbo, ebo;

    bool valid = false;

    int width = -1;
    int height = -1;
    float ratio = -1;

    GLfloat vertices[32] = {
        // Positions          // Texture Coords
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f, // Top Right
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, // Bottom Left
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f  // Top Left
    };

    GLuint indices[6] = {
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };

    glm::mat4 mvp;
    glm::vec4 color = glm::vec4(0.0, 0.0, 0.0, 1.0);

    ITransition* t_in = nullptr;
    ITransition* t_out = nullptr;

    bool should_hide = false;
    bool hidden = false;
    bool transitioning = false;

    void init_shader();
    void init();

    /**
     * Load the given image into this texture.
     * @param path Path to an image.
     * @return true if successful, false otherwise.
     */
    bool load_image(const char*);

public:
    /**
     * Create a new texture.
     * @param cfg Config::Texture to configure this texture
     * @param unit A texture unit used to bind the texture to.
     *             Read up on glActiveTexture for better understanding
     */
    Texture(const Config::Texture& cfg, int unit);
    // disable copy construction
    Texture(const Texture&) = delete;
    ~Texture();

    /**
     * Retrive this textures id.
     * @return The textures id which has been passed while initializing.     */
    const int get_id() {
        return this->tex_unit;
    };

    /**
     * Check if this is a valid texture.
     * @return true if this is a valid texture, false otherwise.
     */
    bool is_valid() {
        return this->valid;
    };

    /**
     * Configure this texture.
     * @param cfg Config::Texture to configure this texture
     */
    void configure(const Config::Texture& cfg);

    /**
     * Set in and out transitions for this texture.
     * @param in Transition used when showing this texture.
     * @param out Transition used when hiding this texture.
     */
    void set_transition(Config::Transition in,
                        Config::Transition out);

    /**
     * Update the transformation matrix of this texture.
     * @param trans glm::mat4 transformation to apply.
     */
    void update_transformation(glm::mat4 trans);

    /**
     * Update the transformation matrix of this texture.
     * @param trans Config::Transformation to apply.
     */
    void update_transformation(Config::Transformation trans);

    /**
     * Update this textures color value.
     * @param color rgba color to apply.
     */
    void update_color(Config::Color color);

    /**
     * Update this textures display mode.
     * @param mode The display mode to apply.
     */
    void update_mode(Config::Texture::Mode mode);

    /**
     * Draw this texture.
     */
    void draw();

    /**
     * Instruct to transition out for all subsequent calls to draw.
     */
    void hide() {
        this->should_hide = true;
    };

    /**
     * Whether or not the out transition is finished.
     * @return true if this texture has been transitioned out, false otherwise.
     */
    bool is_hidden() {
        return this->hidden;
    };

    /**
     * Whether or not a transition is in progress.
     * @return true if currently transitioning out, false otherwise.
     */
    bool is_transitioning() {
        return this->should_hide && ! this->hidden;
    };
};
