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

#pragma once

#include <GL/glew.h>


namespace GL {
    // VBO RAII wrapper
    class Buffer {
        public:
            GLuint id;

            inline Buffer() {
                glGenBuffers(1, &id);
            };

            inline ~Buffer() {
                glDeleteBuffers(1, &id);
            };
            // disable copying
            Buffer(const Buffer&) = delete;

            inline void bind(GLenum target=GL_ARRAY_BUFFER) {
                glBindBuffer(target, id);
            };

            static inline void unbind(GLenum target=GL_ARRAY_BUFFER) {
                glBindBuffer(target, 0);
            };

            inline void operator()() {
                bind();
            };

            inline void tfbind() {
                glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, id);
            };
    };

    // Texture wrapper
    class Texture {
        public:
            GLuint id;

            inline Texture() {
                glGenTextures(1, &id);
            };

            inline ~Texture() {
                glDeleteTextures(1, &id);
            };
            // disable copying
            Texture(const Texture&) = delete;

            inline void bind(GLenum target=GL_TEXTURE_2D) {
                glBindTexture(target, id);
            };

            inline void activate(GLenum texture) {
                glActiveTexture(texture);
            };

            static inline void unbind(GLenum target=GL_TEXTURE_2D) {
                glBindTexture(target, 0);
            };

            inline void operator()() {
                bind();
            };
    };

    // VAO RAII wrapper
    class VAO {
        public:
            GLuint id;

            inline VAO() {
                glGenVertexArrays(1, &id);
            };

            inline ~VAO() {
                glDeleteVertexArrays(1, &id);
            };

            // disable copying
            VAO(const VAO&) = delete;

            inline void bind() {
                glBindVertexArray(id);
            };

            static inline void unbind() {
                glBindVertexArray(0);
            };

            inline void operator()() {
                bind();
            };

    };

    // Shader RAII wrapper
    class Shader {
        public:
            GLuint id;

            inline Shader(const char* code, GLuint type) {
                id = glCreateShader(type);
                glShaderSource(id, 1, &code, nullptr);
                glCompileShader(id);
            };

            inline ~Shader() {
                glDeleteShader(id);
            };

            // disable copying
            Shader(const Shader&) = delete;
    };
}
