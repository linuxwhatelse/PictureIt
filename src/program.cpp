/*
 *  Copyright (C) 2016  Hannes Haberl
 *
 *  This file is part of GLMViz.
 *
 *  GLMViz is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GLMViz is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GLMViz.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "program.hpp"

#include <stdexcept>
#include <string>
#include <vector>


namespace GL {
    Program::Program() {
        this->id = glCreateProgram();
    }

    Program::~Program() {
        glDeleteProgram(this->id);
    }

    void Program::link(Shdrs shaders) {
        // attach compiled shaders
        for (GL::Shader& s : shaders){
            glAttachShader(this->id, s.id);
        }

        glLinkProgram(this->id);

        // check link status and throw if the shaders can't be linked
        GLint link_status;
        glGetProgramiv(this->id, GL_LINK_STATUS, &link_status);

        if (link_status == GL_FALSE){
            GLint length;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);

            std::vector<GLchar> log(length);
            glGetProgramInfoLog(id, length, &length, log.data());

            std::string err(log.begin(), log.end());
            throw std::invalid_argument(err);
        }

        // detach shaders for later cleanup
        for (GL::Shader& s : shaders){
            glDetachShader(id, s.id);
        }
    }

    void Program::link_TF(const size_t n, const char** fb_varyings, Shdrs shaders) {
        // link with transform feedback varyings
        // set TF varyings
        glTransformFeedbackVaryings(id, n, fb_varyings, GL_INTERLEAVED_ATTRIBS);

        link(shaders);
    }
}
