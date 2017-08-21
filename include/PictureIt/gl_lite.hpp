/*
    https://github.com/ApoorvaJ/Papaya/blob/master/src/libpapaya/gl_lite.h

    gl_lite.hpp - Single-header OpenGL function loader

    ---------------------------------------------------------------------------
    USAGE
    ---------------------------------------------------------------------------
    1) Add the following lines in exactly one of your cpp files to compile the
       implementation:

           #define GL_LITE_IMPLEMENTATION
           #include "gl_lite.h"

    2) In all other files in which you want to use OpenGL functions, simply
       include this header file as follows:

           #include "gl_lite.h"

    3) Call gl_lite_init() before using any OpenGL function and after you have
       a valid OpenGL context.
*/
#ifndef GL_LITE_H
#define GL_LITE_H

#include <dlfcn.h>
#define GLDECL // Empty define

#include <GL/gl.h>


#define PICTUREIT_GL_LIST \
    /* ret,        name,                        params */ \
    GLE(GLuint,    CreateProgram,               void) \
    GLE(void,      LinkProgram,                 GLuint program) \
    GLE(void,      UseProgram,                  GLuint program) \
    GLE(void,      DeleteProgram,               GLuint program) \
    GLE(void,      GetProgramiv,                GLuint program, GLenum pname, GLint *params) \
    GLE(void,      GetProgramInfoLog,           GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog) \
    GLE(void,      TransformFeedbackVaryings,   GLuint program, GLsizei count, const char **varyings, GLenum bufferMode) \
    GLE(void,      BeginTransformFeedback,      GLenum primitiveMode) \
    GLE(void,      EndTransformFeedback,        void) \
    \
    GLE(void,      GenBuffers,                  GLsizei n, GLuint *buffers) \
    GLE(void,      BufferData,                  GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage) \
    GLE(void,      BufferSubData,               GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data) \
    GLE(void,      BindBuffer,                  GLenum target, GLuint buffer) \
    GLE(void,      BindBufferBase,              GLenum target, GLuint index, GLuint buffer) \
    GLE(void,      DeleteBuffers,               GLsizei n, const GLuint * buffers) \
    \
    GLE(void,      AttachShader,                GLuint program, GLuint shader) \
    GLE(void,      DetachShader,                GLuint program, GLuint shader) \
    GLE(void,      CompileShader,               GLuint shader) \
    GLE(void,      DeleteShader,                GLuint shader) \
    GLE(GLuint,    CreateShader,                GLenum type) \
    GLE(void,      ShaderSource,                GLuint shader, GLsizei count, const GLchar* const *string, const GLint *length) \
    \
    GLE(void,      GenVertexArrays,             GLsizei n, GLuint *arrays) \
    GLE(void,      BindVertexArray,             GLuint array) \
    GLE(void,      VertexAttribPointer,         GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer) \
    GLE(void,      EnableVertexAttribArray,     GLuint index) \
    GLE(void,      DeleteVertexArrays,          GLsizei n, const GLuint *arrays) \
    GLE(void,      BindFragDataLocation,        GLuint program, GLuint colorNumber, const char * name) \
    \
    GLE(GLint,     GetUniformLocation,          GLuint program, const GLchar *name) \
    GLE(GLint,     GetAttribLocation,           GLuint program, const GLchar *name) \
    \
    GLE(GLint,     GenerateMipmap,              GLenum target) \
    GLE(GLint,     Uniform1i,                   GLint location, GLint v0) \
    GLE(GLint,     Uniform1f,                   GLint location, GLfloat v0) \
    GLE(GLint,     Uniform4fv,                  GLint location, GLsizei count, const GLfloat *value) \
    GLE(GLint,     UniformMatrix4fv,            GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) \
    /* end */

#define GLE(ret, name, ...) \
    typedef ret GLDECL name##proc(__VA_ARGS__); \
    extern name##proc * gl##name;
PICTUREIT_GL_LIST
#undef GLE

bool gl_lite_init();

#endif //GL_LITE_H

// =============================================================================


#ifdef GL_LITE_IMPLEMENTATION

#include <dlfcn.h>

#define GLE(ret, name, ...) name##proc * gl##name;
PICTUREIT_GL_LIST
#undef GLE

bool gl_lite_init() {
    void* libGL = dlopen("libGL.so", RTLD_LAZY);
    if (!libGL) {
        printf("ERROR: libGL.so couldn't be loaded\n");
        return false;
    }

    #define GLE(ret, name, ...) \
            printf("Trying to load: gl" #name "...\n"); \
            if (!gl##name) { \
                gl##name = (name##proc *) dlsym(libGL, "gl" #name); \
                if (!gl##name) { \
                    printf("gl" #name " failed...\n"); \
                    return false; \
                } \
                printf("gl" #name " loaded...\n"); \
            } else { \
                printf("gl" #name " existed...\n"); \
            }
        PICTUREIT_GL_LIST
    #undef GLE

    return true;
}

#undef GL_LITE_IMPLEMENTATION
#endif //GL_LITE_IMPLEMENTATION
