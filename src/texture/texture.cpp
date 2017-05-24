#include "texture/texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_BMP
#include "stb_image.h"

//#include <thread>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Texture::Texture(const Config::Texture& cfg, int unit) : tex_unit(unit) {
    init_shader();
    init();

    configure(cfg);
}

Texture::~Texture() {
    delete this->t_in;
    delete this->t_out;
}

void Texture::init_shader() {
    const char* vertex_shader =
    #include "shader/tex.vert"
    ;
    GL::Shader vs(vertex_shader, GL_VERTEX_SHADER);

    const char* fragment_shader =
    #include "shader/tex.frag"
    ;
    GL::Shader fs(fragment_shader, GL_FRAGMENT_SHADER);

    this->sh.bind_frag_data(0, "finalColor");

    try {
        this->sh.link({fs, vs});
    } catch(std::invalid_argument& e) {
        fprintf(stderr, "Can't link texture shaders!\n");
        fprintf(stderr, "%s\n", e.what());
    }
}

void Texture::init() {
    this->vao.bind();

    this->vbo.bind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices,
                 GL_STATIC_DRAW);

    this->ebo.bind(GL_ELEMENT_ARRAY_BUFFER);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices,
                 GL_STATIC_DRAW);


    // Position attrib
    GLint arg_pos = this->sh.get_attrib("position");
    glVertexAttribPointer(arg_pos, 3, GL_FLOAT, GL_FALSE,
                          5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(arg_pos);

    // TexCoord attrib
    GLint arg_texCoord = this->sh.get_attrib("texCoord");
    glVertexAttribPointer(arg_texCoord, 2, GL_FLOAT, GL_FALSE,
                          5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(arg_texCoord);

    GL::VAO::unbind();
}

void Texture::configure(const Config::Texture& cfg) {
    // Reset values
    this->initial = true;
    this->valid = false;

    this->width = -1;
    this->height = -1;
    this->ratio = -1;

    this->should_hide = false;
    this->hidden = false;

    if (cfg.path.empty())
        return;

    // Threading doesn't work and I have straight up no clue why
    //std::thread t(&Texture::load_image, this, cfg.path.c_str());
    this->load_image(cfg.path.c_str());

    this->update_transformation(cfg.pos);
    this->update_color(cfg.color);

    set_transition(cfg.transition_in, cfg.transition_out);
}

bool Texture::load_image(const char* path) {
    int x, y, n;
    auto data = stbi_load(path, &x, &y, &n, STBI_rgb_alpha);

    this->width = x;
    this->height = y;
    this->ratio = x / y;

    if (data == nullptr) {
        fprintf(stderr, "No image data\n");
        return false;
    }

    this->texture.bind();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GL::Texture::unbind();

    stbi_image_free(data);

    this->valid = true;
    return true;
}

void Texture::set_transition(Config::Transition in, Config::Transition out) {
    this->t_in = TransitionFactory::make_transition(in.type);
    if (this->t_in != nullptr) {
        this->t_in->direction = ITransition::Direction::IN;
        this->t_in->speed = in.speed;
    }

    this->t_out = TransitionFactory::make_transition(out.type);
    if (this->t_out != nullptr) {
        this->t_out->direction = ITransition::Direction::OUT;
        this->t_out->speed = out.speed;
    }
}

void Texture::update_transformation(glm::mat4 t) {
    this->trans = t;
}

void Texture::update_transformation(Config::Transformation t) {
    glm::mat4 trans = glm::ortho(t.Xmin, t.Xmax, t.Ymin, t.Ymax);
    this->update_transformation(trans);
}

void Texture::update_color(Config::Color color) {
    this->color = glm::vec4(color[0], color[1], color[2], color[3]);
}

void Texture::draw() {
    //fprintf(stdout, "valid: %d\n", this->is_valid());
    if (! this->is_valid())
        return;

    // Activate shader
    this->sh.use();

    // Binder texture
    this->texture.activate(GL_TEXTURE0 + this->tex_unit);
    this->texture.bind();
    GLint i_tex = this->sh.get_uniform("tex");
    glUniform1i(i_tex, this->tex_unit);

    glm::mat4 trans = this->trans;
    glm::vec4 color = this->color;

    this->transitioning = false;

    // Transition in (if necessary)
    if (this->t_in != nullptr && ! this->t_in->is_done()) {
        this->transitioning = true;

        if (! this->t_in->is_started())
            this->t_in->start();

        trans = this->t_in->transform(trans);
        color = this->t_in->color(color);

        if (this->t_in->is_done()) {
            this->trans = trans;
            this->color = color;
        }
    }

    // Transition out (if necessary)
    if (this->should_hide) {
        if (this->t_out != nullptr && !this->t_out->is_done()) {
            this->transitioning = true;

            if (! this->t_out->is_started())
                this->t_out->start();

            trans = this->t_out->transform(trans);
            color = this->t_out->color(color);

            if (this->t_in->is_done()) {
                this->trans = trans;
                this->color = color;
            }
        } else {
            this->hidden = true;
        }
    }

    // Tell the shader
    GLint i_trans = this->sh.get_uniform("trans");
    glUniformMatrix4fv(i_trans, 1, GL_FALSE, glm::value_ptr(trans));

    GLint i_texColor = this->sh.get_uniform("texColor");
    glUniform4fv(i_texColor, 1, glm::value_ptr(color));

    // Draw container
    this->vao.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    GL::VAO::unbind();
}

