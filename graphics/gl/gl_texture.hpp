#pragma once

#include <GL/glew.h>
#include "gl_enums.hpp"
#include "../error_handling.hpp"

namespace Graphics {

class GLTexture {
    GLuint handle = 0;
    GLTextureTarget target;
    GLTextureInternalFormat internal_format;
    GLTextureFormat format = GLTextureFormat::rgba;
    GLTextureType type = GLTextureType::type_byte;

public:
    GLTexture(GLTextureTarget target, GLTextureInternalFormat internal_format):
            target(target),
            internal_format(internal_format)
    {};
    ~GLTexture() { if(handle) glDeleteTextures(1, &handle); }

    void create_texture() {
        glGenTextures(1, &handle);
        GLException::check();
    }

    void set_format(GLTextureFormat p_format) { format = p_format; }
    void set_type(GLTextureType p_type) { type = p_type; }

    void bind() {
        glBindTexture((GLenum)target, (GLenum) handle);
        GLException::check();
    }

    void set_image(int width, int height, const void *pixels = nullptr) {
        glTexImage2D((GLenum)target, 0, (GLint)internal_format, width, height, 0, (GLenum)format, (GLenum)type, pixels);
        GLException::check();
    };

    void set_parameter(GLTextureParameter parameter, GLint value) {
        glTexParameteri((GLenum)target, (GLenum)parameter, value);
        GLException::check();
    }

    void set_parameter(GLTextureParameter parameter, GLfloat value) {
        glTexParameterf((GLenum)target, (GLenum)parameter, value);
        GLException::check();
    }

    GLuint get_handle() { return handle; }
};

}