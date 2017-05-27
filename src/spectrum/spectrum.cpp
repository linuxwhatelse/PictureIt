/*
 *    Copyright (C) 2016  Hannes Haberl
 *
 *    This file is part of GLMViz.
 *
 *    GLMViz is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    GLMViz is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with GLMViz.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "spectrum/spectrum.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>

Spectrum::Spectrum(const Config::Spectrum& config, const unsigned s_id) : id(s_id) {
    init_bar_shader();
    init_bar_pre_shader();

    configure(config);

    init_bars();
    init_bars_pre();
}

void Spectrum::draw() {
    /* gravity processing shader */
    sh_bars_pre.use();
    v_bars_pre[tf_index].bind();

    // bind first feedback buffer as TF buffer
    b_fb[tf_index].tfbind();

    // begin transform feedback
    glBeginTransformFeedback(GL_POINTS);
    glEnable(GL_RASTERIZER_DISCARD);

    glDrawArrays(GL_POINTS, 0, output_size);

    // disable TF
    glDisable(GL_RASTERIZER_DISCARD);
    glEndTransformFeedback();

    //undbind feedback buffer
    glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, 0);


    /* render bars */
    sh_bars.use();
    v_bars[tf_index].bind();
    glDrawArrays(GL_POINTS, 0, output_size);

    // switch tf buffers
    tf_index = !tf_index;

    // unbind VAOs
    GL::VAO::unbind();
}

void Spectrum::resize_tf_buffers(const size_t size) {
    b_fb[0].bind();
    glBufferData(GL_ARRAY_BUFFER, size * 3 * sizeof(float), 0, GL_DYNAMIC_DRAW);

    b_fb[1].bind();
    glBufferData(GL_ARRAY_BUFFER, size * 3 * sizeof(float), 0, GL_DYNAMIC_DRAW);
}

void Spectrum::resize_x_buffer(const size_t size) {
    std::vector<float> x_data(size);
    float f_size = (float)size;

    // generate x positions ranging from -1 to 1
    for(unsigned int i = 0; i < size; i++) {
        x_data[i] = (((float) i + 0.5) - (f_size * 0.5)) / (f_size * 0.5);
    }

    b_x.bind();
    glBufferData(GL_ARRAY_BUFFER, x_data.size() * sizeof(float), &x_data[0],
                 GL_STATIC_DRAW);
}

void Spectrum::update_fft(FFT& fft) {
    b_fft.bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, this->output_size * sizeof(fftwf_complex),
                    fft.output[offset]);
}

void Spectrum::update_fft(std::vector<std::shared_ptr<FFT>>& ffts) {
    update_fft(*ffts[this->channel]);
}

void Spectrum::resize_fft_buffer(const size_t size) {
    b_fft.bind();
    glBufferData(GL_ARRAY_BUFFER, this->output_size * sizeof(fftwf_complex), 0,
                 GL_DYNAMIC_DRAW);
}

void Spectrum::configure(const Config::Spectrum& scfg) {
    sh_bars.use();

    // Post compute specific uniforms
    GLint i_width = sh_bars.get_uniform("width");
    glUniform1f(i_width, scfg.bar_width / (float) scfg.get_output_size());

    // set bar color gradients
    GLint i_top_color = sh_bars.get_uniform("top_color");
    glUniform4fv(i_top_color, 1, scfg.top_color.rgba);

    GLint i_bot_color = sh_bars.get_uniform("bot_color");
    glUniform4fv(i_bot_color, 1, scfg.bot_color.rgba);

    GLint i_gradient = sh_bars.get_uniform("gradient");
    glUniform1f(i_gradient, scfg.gradient);


    sh_bars_pre.use();
    // set precompute shader uniforms
    GLint i_fft_scale = sh_bars_pre.get_uniform("fft_scale");
    glUniform1f(i_fft_scale, scfg.get_fft().get_scale());

    GLint i_slope = sh_bars_pre.get_uniform("slope");
    glUniform1f(i_slope, scfg.get_slope() * 0.5);

    GLint i_offset = sh_bars_pre.get_uniform("offset");
    glUniform1f(i_offset, scfg.get_offset() * 0.5);

    GLint i_gravity = sh_bars_pre.get_uniform("gravity");
    glUniform1f(i_gravity, scfg.get_gravity());

    i_slope = sh_lines.get_uniform("slope");
    glUniform1f(i_slope, scfg.get_slope());

    resize(scfg.get_output_size());
    this->offset = scfg.get_data_offset();
    set_transformation(scfg.pos);

    // limit number of channels
    this->channel = scfg.get_channel();
}

void Spectrum::resize(const size_t size) {
    if (size != this->output_size) {
        this->output_size = size;
        resize_tf_buffers(size);
        resize_x_buffer(size);
        resize_fft_buffer(size);
    }
}

void Spectrum::set_transformation(const Config::Transformation& t) {
    // apply simple ortho transformation
    glm::mat4 transformation = glm::ortho(t.Xmin, t.Xmax, t.Ymin, t.Ymax);

    sh_bars.use();
    GLint i_trans = sh_bars.get_uniform("trans");
    glUniformMatrix4fv(i_trans, 1, GL_FALSE, glm::value_ptr(transformation));

    sh_lines.use();
    i_trans = sh_lines.get_uniform("trans");
    glUniformMatrix4fv(i_trans, 1, GL_FALSE, glm::value_ptr(transformation));
}

void Spectrum::init_bar_shader() {
    const char* vertex_shader =
    #include "shader/bar.vert"
    ;
    GL::Shader vs(vertex_shader, GL_VERTEX_SHADER);

    const char* fragment_shader =
    #include "shader/simple.frag"
    ;
    GL::Shader fs(fragment_shader, GL_FRAGMENT_SHADER);

    // geometry shader
    // draw bars
    const char* geometry_shader =
    #include "shader/bar.geom"
    ;
    GL::Shader gs(geometry_shader, GL_GEOMETRY_SHADER);

    // link shaders
    try{
        sh_bars.link({fs, vs, gs});
    } catch(std::invalid_argument& e) {
        std::cerr << "Can't link bar shaders!" << std::endl;
        std::cerr << e.what() << std::endl;
    }
}

void Spectrum::init_bars() {
    for(unsigned i = 0; i < 2; i++) {
        v_bars[i].bind();

        b_x.bind();
        // set x position data in bar VAO
        GLint arg_x_data = sh_bars.get_attrib("x");
        glVertexAttribPointer(arg_x_data, 1, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(arg_x_data);

        // enable the preprocessed y attribute
        b_fb[i].bind();
        GLint arg_y = sh_bars.get_attrib("y");
        glVertexAttribPointer(arg_y, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (const GLvoid*)(2*sizeof(float)));
        glEnableVertexAttribArray(arg_y);

        GL::Buffer::unbind();
        GL::VAO::unbind();
    }
}

void Spectrum::init_bar_pre_shader() {
    const char* vertex_shader =
    #include "shader/bar_pre.vert"
    ;
    GL::Shader vs(vertex_shader, GL_VERTEX_SHADER);

    try{
        const char* varyings[3] = {"v_gravity", "v_time", "v_y"};
        sh_bars_pre.link_TF(3, varyings, {vs});
    } catch(std::invalid_argument& e) {
        std::cerr << "Can't link bar_pre shader!" << std::endl;
        std::cerr << e.what() << std::endl;
    }
}

void Spectrum::init_bars_pre() {
    for(unsigned i = 0; i < 2; i++){
        /* Pre compute shader */
        v_bars_pre[i].bind();

        b_fft.bind();
        // set fft_data buffer as vec2 input for the shader
        GLint arg_fft_output = sh_bars_pre.get_attrib("a");
        glVertexAttribPointer(arg_fft_output, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(arg_fft_output);

        // enable precompute shader attributes
        b_fb[!i].bind();

        GLint arg_gravity_old = sh_bars_pre.get_attrib("gravity_old");
        glVertexAttribPointer(arg_gravity_old, 1, GL_FLOAT, GL_FALSE,
                              3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(arg_gravity_old);

        GLint arg_time_old = sh_bars_pre.get_attrib("time_old");
        glVertexAttribPointer(arg_time_old, 1, GL_FLOAT, GL_FALSE,
                              3 * sizeof(float), (const GLvoid*)(sizeof(float)));
        glEnableVertexAttribArray(arg_time_old);

        GL::Buffer::unbind();
        GL::VAO::unbind();
    }
}

